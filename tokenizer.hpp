#pragma once
#include "System.hpp"

/*
Dans le tokenizer:
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
				if (RawCode[i] == '=' && !(IsInSimpleQuote && IsInDoubleQuote)) IsEquSymb = true;
				if (RawCode[i] == ':' && !(IsInSimpleQuote && IsInDoubleQuote)) IsDPointSymb = true;
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
	public:
		tokenize(std::string RawCode) {
			Arrange(RawCode);
		}
		std::vector<std::string> GetLines() {
			return lines;
		}
	};
}
