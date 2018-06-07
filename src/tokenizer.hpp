#pragma once
#include "System.hpp"
#include "statements.h"
#include "identifier.hpp"

/*

	!! multipass !! define / proc !! --> identifier !! 

- créer les noms de class, fonction, variables par rapport à leur groupe (lC, ...) ;
- commentaire ;
- macros / import
*/

namespace tokenizer {
	class tokenize {
	private:
		std::vector<std::string> lines;
		std::string ReplaceSet(std::string code) {
			std::string ret;
			bool IsInSet = false;
			std::string NameOfSet;
			for (std::string line : System::Text::GetEachLine(code)) {
				NameOfSet = System::Text::replace(NameOfSet, " ", "");
				if (System::Text::StartsWih(line, "set ")) {
					NameOfSet = System::Text::replace(line, "set ", "");
					IsInSet = true;
					ret += "{\n";
					continue;
				}
				if (IsInSet && line == "{") continue;
				if (IsInSet && line == "}") IsInSet = false;
				// else
				if (IsInSet && System::Vector::split(line, " ")[1] == "=") ret += NameOfSet + "." + line + '\n';
				else ret += line + '\n';
			}
			return ret;
		}
		void Arrange(std::string RawCode) {
			std::string temp;
			std::string tmp;
			bool IsInSimpleQuote = false; // '
			bool IsInDoubleQuote = false; // "
			bool IsInContainer = false;   // {}
			bool IsEquSymb = false;		  // =
			bool IsDPointSymb = false;	  // :
			for (int i = 0; i < RawCode.length(); ++i) {
				// check quotations mark
				if (RawCode[i] == '"' && !IsInDoubleQuote && !IsInSimpleQuote) { IsInDoubleQuote = true; goto next; }
				if (RawCode[i] == '\'' && !IsInSimpleQuote && !IsInDoubleQuote) { IsInSimpleQuote = true; goto next; }
				if (RawCode[i] == '"' && IsInDoubleQuote) { IsInDoubleQuote = false; goto next; }
				if (RawCode[i] == '\'' && IsInSimpleQuote) { IsInSimpleQuote = false; goto next; }
				// continue
				if (RawCode[i] == '{' && !(IsInDoubleQuote && IsInSimpleQuote)) {
					temp += (tmp)+'\n';
					tmp.clear();
					temp += (System::Text::CharToString(RawCode[i])) + '\n';
					IsInContainer = true;
					continue;
				}
				if (RawCode[i] == '\t' || (RawCode[i] == ' ' && RawCode[i - 1] == ' ') && !(IsInDoubleQuote && IsInSimpleQuote)) continue;
				//if (RawCode[i] == '=' && !(IsInSimpleQuote && IsInDoubleQuote)) IsEquSymb = true;
				//if (RawCode[i] == ':' && !(IsInSimpleQuote && IsInDoubleQuote)) IsDPointSymb = true;
			next:
				if (IsInDoubleQuote || IsInSimpleQuote) tmp += RawCode[i];
				else if (!(IsInDoubleQuote || IsInSimpleQuote) && RawCode[i] == ';') {
					temp += (tmp)+'\n';
					tmp.clear();
				}
				else if (!(IsInDoubleQuote || IsInSimpleQuote) && RawCode[i] == '}') {
					temp += "}\n";
				}
				else if (IsEquSymb || IsDPointSymb) {
					std::string add;
					if (IsEquSymb) add = "=";
					if (IsDPointSymb) add = ":";
					if (RawCode[i - 1] == ' ' && RawCode[i + 1] == ' ') tmp += add;
					else if (RawCode[i - 1] != ' ' && RawCode[i + 1] != ' ') tmp += " " + add + " ";
					else if (RawCode[i - 1] == ' ' && RawCode[i + 1] != ' ') tmp += add + " ";
					else if (RawCode[i - 1] != ' ' && RawCode[i + 1] == ' ') tmp += " " + add;
					IsEquSymb = false;
					IsDPointSymb = false;
				}
				else tmp += RawCode[i];
			}
			temp = ReplaceSet(temp);
			lines = System::Text::GetEachLine(temp);
		}
		// GET -> Get Each Token
		std::vector<std::string> GET(std::string subject) {
			std::vector<std::string> ret;
			std::string tmp;
			bool IsSimpleQuote = false;
			bool IsDoubleQuote = false;
			for (int i = 0; i < subject.length(); ++i) {
				char next = '\0', before = '\0', lbefore = '\0', chr = subject[i];
				if (i < subject.length()) next = subject[i + 1];
				if (i > 0) before = subject[i - 1];
				if (i > 1) lbefore = subject[i - 2];
				bool IsInStc = (IsSimpleQuote || IsDoubleQuote); // Check if chr is in a string / char quotation marks

				 /* --- symbols --- */
				if (!IsInStc && lbefore == '-' && before == '-' && chr == '>') { ret.pop_back(); ret.push_back("-->"); }	// -->
				else if (!IsInStc && before == '&' && chr == '&') ret.push_back("&&");										// &&
				else if (!IsInStc && before == '|' && chr == '|') ret.push_back("||");										// ||
				else if (!IsInStc && before == '!' && chr == '=') ret.push_back("!=");										// !=
				else if (!IsInStc && before == '=' && chr == '=') ret.push_back("==");										// ==
				else if (!IsInStc && before == '+' && chr == '=') ret.push_back("+=");										// +=
				else if (!IsInStc && before == '-' && chr == '=') ret.push_back("-=");										// -=
				else if (!IsInStc && before == '*' && chr == '=') ret.push_back("*=");										// *=
				else if (!IsInStc && before == '/' && chr == '=') ret.push_back("/=");										// /=
				else if (!IsInStc && before == '%' && chr == '=') ret.push_back("%=");										// %=
				else if (!IsInStc && before == '>' && chr == '=') ret.push_back(">=");										// >=
				else if (!IsInStc && before == '<' && chr == '=') ret.push_back("<=");										// <=
				else if (!IsInStc && before == '+' && chr == '+') ret.push_back("++");										// ++
				else if (!IsInStc && before == '-' && chr == '-' && next != '>') ret.push_back("--");						// --
				else if (!IsInStc && before == '-' && chr == '>') ret.push_back("->");										// ->

				else if (!IsInStc && chr == '+' && next != '+' && next != '=') ret.push_back("+");							// +
				else if (!IsInStc && chr == '-' && next != '-' && next != '=') ret.push_back("-");							// -
				else if (!IsInStc && chr == '*' && next != '*' && next != '=') ret.push_back("*");							// *
				else if (!IsInStc && chr == '/' && next != '/' && next != '=') ret.push_back("/");							// /
				else if (!IsInStc && chr == '%' && next != '%' && next != '=') ret.push_back("%");							// %

				else if (!IsInStc && chr == '|' && next != '|') ret.push_back("|");											// |
				else if (!IsInStc && chr == '&' && next != '&') ret.push_back("&");											// &
				else if (!IsInStc && chr == '!' && next != '=') ret.push_back("!");											// !
				else if (!IsInStc && chr == '=' && next != '=') ret.push_back("=");											// =

				else if (!IsInStc && chr == '~') ret.push_back("~");														// ~
				else if (!IsInStc && chr == '@') ret.push_back("@");														// @
				else if (!IsInStc && chr == '.' && !isdigit(before) && !isdigit(next)) ret.push_back(".");					// .
				else if (!IsInStc && chr == ':') ret.push_back(":");														// :
				else if (!IsInStc && chr == '!') ret.push_back("~");														// ~
				else if (!IsInStc && chr == ',') ret.push_back(",");														// ,
				else if (!IsInStc && chr == '(') ret.push_back("(");														// (
				else if (!IsInStc && chr == ')') ret.push_back(")");														// )
				else if (!IsInStc && chr == '[') ret.push_back("[");														// [
				else if (!IsInStc && chr == ']') ret.push_back("]");														// ]
				else if (!IsInStc && chr == '{') ret.push_back("{");														// {
				else if (!IsInStc && chr == '}') ret.push_back("}");														// }

				else if (chr == '"' && !IsDoubleQuote && !IsSimpleQuote) IsDoubleQuote = true;
				else if (chr == '"' && IsDoubleQuote) { IsDoubleQuote = false; ret.push_back(tmp += chr); tmp.clear(); }
				else if (chr == '\'' && !IsSimpleQuote && !IsDoubleQuote) IsSimpleQuote = true;
				else if (chr == '\'' && IsSimpleQuote) { IsSimpleQuote = false; ret.push_back(tmp += chr); tmp.clear(); }
				
				/* --- string / char / number --- */
				if (IsInStc) tmp += chr;
				if (!IsInStc && isdigit(chr)) tmp += chr;
				if (!IsInStc && isdigit(before) && chr == '.' && isdigit(next)) tmp += ".";
				else if (!IsInStc && isdigit(chr) && !isdigit(next) && next != '.') { ret.push_back(tmp); tmp.clear(); }

				/* identifier / keywords */
				if (!IsInStc && ret.empty()) tmp += chr;
				if (!IsInStc && keywords::IsKeyword(tmp) && next == ' ') { ret.push_back(tmp); tmp.clear(); }
			}
			return ret;
		}
	public:
		tokenize(std::string RawCode) {
			Arrange(RawCode);
		}
		std::vector<std::string> GetLines() {
			return lines;
		}
		std::vector<std::string> GetTokens() {
			std::vector<std::string> ret;
			for (std::string tmp : lines)
				for (std::string token : GET(tmp))
					ret.push_back(token);
			return ret;
		}
	};
}
