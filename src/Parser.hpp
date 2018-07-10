#pragma once
#include "System.hpp"
#include "statements.h"
#include "AbstractName.hpp"
#include "lexer.hpp"
#include "preprocessor.hpp"

/*
	https://thinkingeek.com/2016/01/10/tiny-gcc-front-part-4/
*/

class Parser {
private:
	Preprocessor::Defines ListOfDefines;
private:
	typedef Lexer::analyse LTOK;
	typedef keywords::keywords Token;
	std::vector<LTOK> Tokens;
	std::vector<LTOK> CurrentLine;
	int Current_t = 0;
private:
	std::string CurrentFile{ "" };
	const std::string InvalidNumberOfToken = "Invalid number of token";
private:
	std::string GetExpr() {
		std::string ret;
		for (LTOK token : CurrentLine) ret += token.value + " ";
		return ret;
	}
private:
	void Parse_Define() {
		/// Define ::= define IDENTIFIER [VALUE];
		if (!identifier::IsCorrectIdentifier(CurrentLine[1].value)) return;
		if (CurrentLine.size() == 4) {
			if (ListOfDefines.IsAlreadyExist(CurrentLine[2].value))
				ListOfDefines.AppendDef(CurrentLine[1].value, ListOfDefines.GetValueOf(CurrentLine[1].value));
			ListOfDefines.AppendDef(CurrentLine[1].value, CurrentLine[2].value);
		}
		if (CurrentLine.size() == 3) ListOfDefines.AppendDef(CurrentLine[1].value);
		if (CurrentLine.size() >= 5 || CurrentLine.size() <= 2)
			LogMessage::ErrorAt(InvalidNumberOfToken, GetExpr().size() - 2, GetExpr(), 0, CurrentFile);
	}
	void Parse_VariableDeclaration() {
		/*
			Utiliser le vecteur CurrentLine pour analyser la ligne entière à partir du premier jeton, étant ici "var".
		*/
	}
private:
	void UnexpectedToken(LTOK token, int position, int line = -1) {
		std::string tmp_line;
		for (int i = Current_t; i < Tokens.size(); ++i)
			if (Tokens.at(i).keyword != Token::ENDLINE)
				tmp_line += Tokens.at(i).value + " ";
		LogMessage::ErrorAt("Unexpected '" + keywords::ToString(token.keyword) + "'", token.value.length(), tmp_line, position, CurrentFile, line);
	}
	void ParseStatement() {
		if (Current_t > Tokens.size()) return;
		const Token token = Tokens.at(Current_t).keyword;
		switch (token) {
		case Token::DEFINE:
			Parse_Define();
			break;
		case Token::GOTO:

			break;
		case Token::PROC:

			break;
		case Token::SIZEOF:
			
			break;
		case Token::VAR:

			break;
		case Token::FUNC:

			break;
		case Token::RET:

			break;
		case Token::DATA:

			break;
		case Token::ENUM:

			break;
		case Token::STRUCTURE:

			break;
		case Token::RETURN:

			break;
		case Token::CONTINUE:

			break;
		case Token::SET:

			break;
		case Token::AWAIT:

			break;
		case Token::NAMESPACE:

			break;
		case Token::STATIC:

			break;
		case Token::PRIVATE:

			break;
		case Token::PUBLIC:

			break;
		case Token::UPON:

			break;
		case Token::IT:

			break;
		case Token::IF:

			break;
		case Token::ELIF:

			break;
		case Token::ELSE:

			break;
		case Token::WHILE:

			break;
		case Token::FOR:

			break;
		case Token::IDENTIFIER:

			break;
		case Token::NUMBER:
			
			break;
		case Token::EXTERN:

			break;
		case Token::MATCH:

			break;
		case Token::TRY:

			break;
		case Token::THROW:

			break;
		case Token::CATCH:

			break;
		case Token::FREE:

			break;
		case Token::_DELETE:

			break;
		case Token::DATA_ENUM_STRUCTURE_OBJCALL:

			break;
		case Token::POINTER_ADRESS:

			break;
		case Token::POINTER_VALUE:

			break;
		case Token::INC:

			break;
		case Token::DEC:

			break;
		default:
			// proc / define
			UnexpectedToken(Tokens[Current_t], 0);
			break;
		}
	}
private:
	void Parse() {
		for (LTOK token : Tokens) {
			if (Lexer::IsFileIdentifier(Tokens[Current_t].value)) {
				CurrentFile = Tokens[Current_t].value;
				++Current_t;
				continue;
			}
			CurrentLine.push_back(token);
			if (token.keyword == Token::ENDLINE) {
				Current_t -= CurrentLine.size() - 1;
				ParseStatement();
				Current_t += CurrentLine.size() - 1; // Current_t += CurrentLine.size() + 1;
				CurrentLine.clear();
			}
			++Current_t;
		}
	}
public:
	Parser(std::vector<LTOK> tokens) {
		if (!tokens.empty()) {
			Tokens = tokens;
			Parse();
		}
		else LogMessage::LogMessage("Unable to parse code");
	}
};
