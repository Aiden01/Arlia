#pragma once
#include "System.hpp"
#include "statements.h"
#include "identifier.hpp"

namespace tokenizer {
	static std::vector<std::string> GetTokens(std::string subject) {
		std::vector<std::string> ret;
		std::string tmp;
		std::string ident;
		bool IsSimpleQuote = false;	// check string --> ".."
		bool IsDoubleQuote = false;	// check char   --> '..'
		for (int i = 0; i < subject.length(); ++i) {
			char next = '\0', before = '\0', lbefore = '\0', chr = subject[i];
			if (i < subject.length()) next = subject[i + 1];
			if (i > 0) before = subject[i - 1];
			if (i > 1) lbefore = subject[i - 2];
			bool IsInStc = (IsSimpleQuote || IsDoubleQuote); // Check if chr is in a string / char quotation marks
			if (!IsInStc && chr == ' ' || chr == '\t') continue;

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
			else if (!IsInStc && chr == '-' && next != '-' && next != '=' && next != '>') ret.push_back("-");			// -
			else if (!IsInStc && chr == '*' && next != '*' && next != '=') ret.push_back("*");							// *
			else if (!IsInStc && chr == '/' && next != '/' && next != '=') ret.push_back("/");							// /
			else if (!IsInStc && chr == '%' && next != '%' && next != '=') ret.push_back("%");							// %

			else if (!IsInStc && chr == '|' && next != '|') ret.push_back("|");											// |
			else if (!IsInStc && chr == '&' && next != '&') ret.push_back("&");											// &
			else if (!IsInStc && chr == '!' && next != '=') ret.push_back("!");											// !
			else if (!IsInStc && chr == '=' && next != '=') ret.push_back("=");											// =

			else if (!IsInStc && chr == '>') ret.push_back(">");														// >
			else if (!IsInStc && chr == '<') ret.push_back("<");														// <

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
			else if (!IsInStc && chr == ';') ret.push_back(";");														// ;

			else if (chr == '"' && !IsDoubleQuote && !IsSimpleQuote) IsDoubleQuote = true;
			else if (chr == '"' && IsDoubleQuote) { IsDoubleQuote = false; ret.push_back(tmp += chr); tmp.clear(); }
			else if (chr == '\'' && !IsSimpleQuote && !IsDoubleQuote) IsSimpleQuote = true;
			else if (chr == '\'' && IsSimpleQuote) { IsSimpleQuote = false; ret.push_back(tmp += chr); tmp.clear(); }

			/* --- string / char / number --- */
			if (IsInStc) tmp += chr;
			if (IsDoubleQuote) if (tmp[0] != '"')  tmp.insert(0, "\"");
			if (IsSimpleQuote) if (tmp[0] != '\'')  tmp.insert(0, "'");
			if (!IsInStc && isdigit(chr)) tmp += chr;
			if (!IsInStc && isdigit(before) && chr == '.' && isdigit(next)) tmp += ".";
			else if (!IsInStc && isdigit(chr) && !isdigit(next) && next != '.') { ret.push_back(tmp); tmp.clear(); }

			/* identifier / keywords */
			if (!IsInStc && !isdigit(tmp[0]) && (isalpha(chr) || (isdigit(chr) && !ident.empty())) && !keywords::IsKeyword(tmp))
				ident += chr;
			if (!IsInStc && !isalpha(next)) {
				if (!keywords::IsKeyword(ident) && !ident.empty()) {
					ret.push_back(ident);
					ident.clear();
				}
			}

			if (!IsInStc && ret.empty()) tmp += chr;
			if (!IsInStc && keywords::IsKeyword(tmp) && (next == ' ' || next == '\t')) { ret.push_back(tmp); tmp.clear(); ident.clear(); }

			if (keywords::IsKeyword(ident) && (next == ' ' || next == '\t')) { ret.push_back(ident); ident.clear(); }

		}
		return ret;
	}
}
