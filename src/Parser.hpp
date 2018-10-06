#pragma once
#include "System.hpp"
#include "Lexer.hpp"
#include "Errors.hpp"
#include "Expression.hpp"
#include "DefineStmt.hpp"
#include "Maker.hpp"

#include "StatementTypes.hpp"

namespace Parser {
	class parser {

		DefineStatement Define;
		Maker maker;
		Exception exception;

		void import(bool, Expr&, Lexer&, Lexer&, token_t, std::vector<std::string>&);

		void AppendToAst(StatementTypes::StatementTypes, Expr);
		void AppendToAst_StartingBlock(); // '{'
		void AppendToAst_EndingBlock();   // '}'

		int prestmt_NoDefined(token_t&, Expr&, TokenList::TokenList, StatementTypes::StatementTypes&, Lexer&);
		void SetStatement(StatementTypes::StatementTypes, Lexer &, Lexer &, std::vector<std::string> &,
			token_t, Expr &, bool);
		void UpdateLexerDefine(Lexer&, Lexer&);

		StatementTypes::StatementTypes DetermineStatement(TokenList::TokenList, Expr, Lexer&);

	public:
		void parse(Lexer&, std::vector<std::string>&, std::string = "");
	};
}
