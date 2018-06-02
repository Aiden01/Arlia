#pragma once
#include "System.hpp"
#include "MathAst.hpp"
#include "AssemblerInsert.hpp"

#define ERROR "ùrr$r" // Swag error

namespace MathExpression {
	class SetExpression {
	public:
		/// Utiliser .next pour les paramètres de fonction
		std::string reg = "eax";
		void SetReg(std::string r) {
			reg = r;
		}
	private:
		bool IsAllCharSame(std::string subject) {
			return subject.find_first_not_of(subject[0]) == std::string::npos;
		}

		std::string TakeASTexp(std::string Expression) {
			MathExpressionAst::Exp *tree = MathExpressionAst::StringToExpression(Expression);
			tree->Build();
			tree->release();
			delete tree;
			std::string ret = System::Text::ReplaceMultiSpace(MathExpressionAst::StrToExp);
			MathExpressionAst::StrToExp.clear();
			return ret;
		}
		std::string CorrSymb(char Symb, int line) {
			switch (Symb) {
			case '+':
				return "add ";
			case '-':
				return "sub ";
			case '*':
				return "imul ";
			case '/':
				return "div ";
			case '%':
				return "mod ";
			case '^':
				return "and ";
			default:
				System::Display::ErrorMessage("Invalid token : '" + System::Text::CharToString(Symb) + "'", line);
				return ERROR;
				break;
			}
		}
		std::string Correspondant(std::string foliage, int line, std::string reg, bool mov) {
			
			std::string ret;
			std::vector<std::string> tokens = System::Text::GetEachWord(foliage);

			// Defines
			/*int ind = 0;
			for (string str : tokens) {
				tokens[ind] = ReplaceByDefineValue(str);
				++ind;
			}*/

			/*
			Nbr: 0 --> exit											(/)
			Nbr: 1 --> error										(a); (+)
			Nbr: 2 --> Symbol & Number/Variable						(+ a); (+ 3)
			Nbr: 3 --> Symbol & Number/Variable & Number/Variable	(+ a b); (+ a 3); (+ 3 a)
			Nbr: +4 -> error										(?)
			*/

			if (tokens.size() == 0) return "";
			if (tokens.size() <= 1) {
				System::Display::ErrorMessage("Invalid number of tokens", line);
				return "";
			}
			if (tokens.size() >= 4) {
				System::Display::ErrorMessage("Invalid number of tokens", line);
				return "";
			}

			if (mov) {
				if (System::Text::IsNumeric(tokens[1]))
					ret += "mov " + AssemblerInsert::OperatorIdentifier(sizeof(size_t)) + " " + reg + ", " + tokens[1] + "\n";
				else if (!System::Text::ContainsSpecialChar(tokens[1], "_"))
					ret += "mov " + AssemblerInsert::OperatorIdentifier(sizeof(size_t)) + " " + reg + ", " + "[" + tokens[1] + "]" + "\n";
			}

			if (tokens.size() == 2) {
				if (!System::Text::ContainsSpecialChar(tokens[1], "_"))
					ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + "[" + tokens[1] + "]" + "\n";
				else if (System::Text::IsNumeric(tokens[1]))
					ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + tokens[1] + "\n";
			}
			if (tokens.size() == 3) {
				if (!mov) {
					if (!System::Text::ContainsSpecialChar(tokens[1], "_"))
						ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + "[" + tokens[1] + "]" + "\n";
					else if (System::Text::IsNumeric(tokens[1]))
						ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + tokens[1] + "\n";
				}

				if (!System::Text::ContainsSpecialChar(tokens[2], "_"))
					ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + "[" + tokens[2] + "]" + "\n";
				else if (System::Text::IsNumeric(tokens[2]))
					ret += CorrSymb(System::Text::StringToChar(tokens[0]), line) + " " + reg + ", " + tokens[2] + "\n";
			}

			if (!mov) ret = ret.substr(ret.find("\n"), ret.length());

			return ret;
		}
		std::string FasmMathWithVariable(std::string expression, bool tab = true, int line = -1, bool mov = true) {
			std::string ast = TakeASTexp(expression);
			std::string temp;
			int indx = 0;
			for (std::string str : System::Text::EachItemInDelimiters(ast, '(', ')')) {
				std::string tmp = System::Text::ReplaceMultiSpace(str.substr(1, str.size() - 2));
				if (indx == 0) temp += System::Text::ReplaceMultiSpace(Correspondant(tmp, line, reg, mov));
				else temp += System::Text::ReplaceMultiSpace(Correspondant(tmp, line, reg, true));
				++indx;
			}
			indx = 0;
			std::string ret;
			for (std::string line : System::Text::GetEachLine(temp)) {
				if (indx == 0) ret += line + "\n";
				else if (!System::Text::StartsWih(line, "mov ")) ret += line + "\n";
				++indx;
			}
			if (tab) {
				std::string retab;
				for (std::string str : System::Text::GetEachLine(ret))
					retab += "\t" + str + "\n";
				return retab;
			}
			return ret;
		}
	public:
		std::string SetFasmMathInstruction(std::string expression, bool tab = true, int line = -1, bool mov = true) {
			expression = System::Text::replace(expression, " ", "");
			if (expression.empty()) return "";
			bool VarValue;
			System::Math eval;
			if (System::Math::IsMathExpression(expression)) VarValue = true; else VarValue = false;
			if (!System::Math::IsMathExpression(expression)) {
				System::Display::ErrorMessage("This is not a valid math expression : '" + expression + "'", line);
				return ERROR;
			}
			else {
				for (std::string str : System::Vector::MultiSplit(expression, "()+-*/%^", true)) {
					/*if (ReplaceByDefineValue(str) == str && !IsMathSym(str[0]) && !IsNumeric(str)) {
						if (IsCorrectVarName(str))
							VarValue = true;
						else {
							InvalidValue(5, str);
							return ERROR;
						}
					}*/
				}
				if (!VarValue) {
					std::string ret = std::to_string(eval.evaluate(expression, line));
					return System::Text::DeleteUnnecessaryZeros(ret);
				}
				else return FasmMathWithVariable(expression, tab, line, mov);
			}
		}
	};
}
