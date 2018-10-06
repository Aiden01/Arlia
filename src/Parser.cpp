#include "Parser.hpp"
#include "Expression.hpp"
#include "ParserFuncs.hpp"

/*
	  -- Primary parser --

	+-----------------------------------------------------------------------------------------------------------+
	| This file contains the main parser implementation.                                                        |
	| Its purpose is to make the construction and analysis of the AST simpler by                                |
	| determining the declarations and code blocks.                                                             |
	| Once a token or token suite is recognized, it and the expression that follows it are sent to the AST.     |
	| The primary parser also allows file import and preprocessor management.                                   |
	| Once the primary parser step is passed for the instruction being analyzed, the lexer is no longer useful. |
	| Only expressions will count and will be stored.                                                           |
	| The main function of this file is Parser::parser::parse().                                                |
	| All other functions here revolve around this function;                                                    |
	| the 'prestmt' functions are sub-procedures of the main function of this file.                             |
	+-----------------------------------------------------------------------------------------------------------+
*/

// Checks if the given expression match with the call of function
bool IsFunction(Expr expr) {
	Parser::TypeOfFuncCall TypeOfFunc = Parser::TypeOfFunctionCall(expr);
	return (
		TypeOfFunc == Parser::TypeOfFuncCall::NamespaceObject_Function
		|| TypeOfFunc == Parser::TypeOfFuncCall::Namespace_Function
		|| TypeOfFunc == Parser::TypeOfFuncCall::Object_Function
		|| TypeOfFunc == Parser::TypeOfFuncCall::_Function
		&& TypeOfFunc != Parser::TypeOfFuncCall::UNKNOWN
		);
}
bool IsVariableEditingSymbol(token_t token) {
	return
		(
			TokenList::IsSymbol(token.value)
			&& token.type != TokenList::DOUBLEPOINT
			&& token.type != TokenList::SUCHAS
			&& token.type != TokenList::AND
			&& token.type != TokenList::OR
			&& token.type != TokenList::LEFT_PARENTHESIS
			&& token.type != TokenList::RIGHT_PARENTHESIS
			&& token.type != TokenList::LEFT_BRACE
			&& token.type != TokenList::RIGHT_BRACE
			&& token.type != TokenList::POINTER_ADRESS
			&& token.type != TokenList::TERNARY
			&& token.type != TokenList::DOT
			&& token.type != TokenList::ISEQUAL
			&& token.type != TokenList::ISNOTEQUAL
			&& token.type != TokenList::GREATER
			&& token.type != TokenList::GREATEREQ
			&& token.type != TokenList::LESS
			&& token.type != TokenList::LESSEQ
			&& token.type != TokenList::OPPOSITE
			&& token.type != TokenList::NAMESPACE_CALLING
			&& token.type != TokenList::COMMA
			/* Simple mathematical symbols do not affect values if they are not from assignment */
			&& token.type != TokenList::PLUS
			&& token.type != TokenList::MINUS
			&& token.type != TokenList::TIME
			&& token.type != TokenList::DIVIDE
			&& token.type != TokenList::MODULO
			&& token.type != TokenList::POWER
			&& token.type != TokenList::_
		);
}
StatementTypes::StatementTypes Parser::parser::DetermineStatement(TokenList::TokenList TokenType, Expr expr,
	Lexer &lexer) {
	if (expr.empty()) return StatementTypes::NoDefined;
	switch (TokenType) {
	case TokenList::NOTHING: return StatementTypes::Nothing;			// -1
	case TokenList::VAR: return StatementTypes::VariableDecl;			// var
	case TokenList::FUNC: return StatementTypes::FunctionDecl;			// func
	case TokenList::LET: return StatementTypes::LetDecl;				// let
	case TokenList::TYPE: return StatementTypes::TypeDecl;				// type
	case TokenList::NAMESPACE: return StatementTypes::NamespaceDecl;	// namespace
	case TokenList::WHILE: return StatementTypes::WhileStmt;			// while
	case TokenList::FOR: return StatementTypes::ForStmt;				// for
	case TokenList::RETURN: return StatementTypes::ReturnStmt;			// return
	case TokenList::CONTINUE: return StatementTypes::ContinueStmt;		// continue
	case TokenList::IF: return StatementTypes::IfStmt;					// if
	case TokenList::ELIF: return StatementTypes::ElifStmt;				// elif
	case TokenList::ELSE: return StatementTypes::ElseStmt;				// else
	case TokenList::MATCH: return StatementTypes::MatchStmt;			// match
	case TokenList::EXTERNE: return StatementTypes::ExterneStmt;		// externe
	case TokenList::THROW: return StatementTypes::ThrowStmt;			// throw
	case TokenList::TRY: return StatementTypes::TryStmt;				// try
	case TokenList::CATCH: return StatementTypes::CatchStmt;			// catch
	case TokenList::_DELETE: return StatementTypes::DeleteStmt;			// delete
	case TokenList::FREE: return StatementTypes::FreeStmt;				// free
	case TokenList::DEFINE: return StatementTypes::DefineStmt;			// define
	case TokenList::IMPORT: return StatementTypes::ImportStmt;			// import

	case TokenList::PRIVATE: return StatementTypes::PrivateMember;		// private
	case TokenList::PUBLIC: return StatementTypes::PublicMember;		// public

	default:
		if (expr.size() > 2 && expr.back().type == TokenList::ENDLINE) expr.pop_back(); // erase potential ';' for analyse
		if (expr.size() >= 3) if (IsFunction(expr)) return StatementTypes::FunctionCalling;
		if (expr.size() == 1) return StatementTypes::SingleToken;
		if (expr[0].type == TokenList::IDENTIFIER
			&& IsVariableEditingSymbol(expr[1]))
			return StatementTypes::IdentifierEdit;
		else if (expr[1].type == TokenList::IDENTIFIER
			&& IsVariableEditingSymbol(expr[0]))
			return StatementTypes::IdentifierEdit;
		return StatementTypes::UNKNOWN;
	}
}
void Parser::parser::UpdateLexerDefine(Lexer &lexer, Lexer &LocalLexer) {
	LocalLexer.Define.AppendRange(lexer.Define);
	lexer.Define = LocalLexer.Define;
}

void Parser::parser::AppendToAst(StatementTypes::StatementTypes StatementType, Expr expr) {
	maker.make(StatementType, expr);
}
void Parser::parser::AppendToAst_StartingBlock() {
	maker.startBlock();
}
void Parser::parser::AppendToAst_EndingBlock() {
	maker.endBlock();
}

#define LEXER		(IsHeader ? LocalLexer : lexer)
#define BREAK	    1

bool MustBreak(Lexer &lexer) {
	return (lexer.eof || !lexer.CanContinue && !lexer.peekchr() == EOF);
}
void Parser::parser::import(bool IsHeader, Expr &expr, Lexer &lexer, Lexer &LocalLexer, token_t token,
	std::vector<std::string> &HeaderImported) {
	System::Vector::push_range(expr, LEXER.GetLine());
	std::string HeaderFilename = GetImportFilename(expr);
	std::string HeaderPath = (LEXER.peekt().position.filename);
	HeaderPath = HeaderPath.substr(0, HeaderPath.find_last_of("/\\"));

	if (!System::Text::contains(HeaderPath, "\\")) HeaderPath.clear();
	else HeaderPath += "\\";

	if (System::Vector::Contains(HeaderImported, HeaderFilename)) {
		// exception.ThrowError(exception.E0083, HeaderFilename);
		expr.clear();
		return;
	}

	if (CanImport(HeaderPath + HeaderFilename)) parse(LocalLexer, HeaderImported, (HeaderPath + HeaderFilename));
	else ImportFailed(("..\\" + HeaderPath + HeaderFilename), expr[0].position, exception);

	UpdateLexerDefine(lexer, LocalLexer);
	expr.clear();
}

// Erase 'bad' tokens: '\0'
void CleanExpr(Expr &expr) {
	token_t token;
	for (size_t i = 0; i < expr.size(); ++i) {
		token = expr[i];
		if (token.type == TokenList::NOTHING
			|| System::Text::RemoveFirstSpaces(token.value).empty()
			|| token.value[0] == -1)
			expr.erase(expr.begin() + i);
	}
	if (expr.back().value[0] == 0) expr.pop_back();
}
// Gets the new token from the lexer, and adds it to the current expression
void LexPushInExp(token_t &token, Expr &expr, Lexer &lexer) {
	token = lexer.next();
	expr.push_back(token);
}
// Gets all expression until ';' or '\0' or '{' or '}'
void pushexpr(Lexer &lexer, Expr &expr) {
	token_t inst_t;
	do {
		if (lexer.peekt().type == TokenList::LEFT_BRACE
			|| lexer.peekt().type == TokenList::RIGHT_BRACE)
			break;
		LexPushInExp(inst_t, expr, lexer);
	} while (inst_t.type != TokenList::ENDLINE && !lexer.eof);
}
// Gets all the expression to its end
void prestmt_single_isin_true(token_t &inst_t, Expr &expr, Lexer &lexer) {
	do LexPushInExp(inst_t, expr, lexer);
	while (inst_t.type != TokenList::ENDLINE && !lexer.eof);
}
// Builds the current expression if the original token is unidentified (single)
void prestmt_single_isin(token_t &token, Lexer &lexer, Expr &expr) {
	while (token.type != TokenList::DOUBLEPOINT || token.type != TokenList::DOT) {
		LexPushInExp(token, expr, lexer);
		if (token.type == TokenList::DOUBLEPOINT) prestmt_single_isin_true(token, expr, lexer);
		else if (token.type == TokenList::DOT); /// en attente de besoins
		if (lexer.eof) break;
		if (token.type == TokenList::ENDLINE) break;
		if (lexer.peekt().type == TokenList::RIGHT_BRACE) break;
	}
}
// Identifies the single token to determine if it is the origin of a valid expression
void prestmt_single(token_t &token, Expr &expr, Lexer &lexer) {
	if (token.type == TokenList::NAMESPACE_CALLING	// Calling from a namespace
		|| token.type == TokenList::IDENTIFIER		// Call from an object (included variable)
		|| token.type == TokenList::NUMBER			// Calling from a decimal type literal object
		|| token.type == TokenList::STRING			// Calling from a string type literal object
		|| token.type == TokenList::CHAR)			// Calling from a char type literal object
		prestmt_single_isin(token, lexer, expr);
	else while (token.type != TokenList::ENDLINE && !lexer.eof) LexPushInExp(token, expr, lexer); /// le 'else' n'y était pas
	while (expr.back().type == TokenList::NOTHING) expr.pop_back();
}
// Throw an error if the expression is invalid
void prestmt_rsingle(Expr &expr, Lexer &lexer, Exception exception) {
	token_t inst_t;
	while (inst_t.type != TokenList::ENDLINE && !lexer.eof) LexPushInExp(inst_t, expr, lexer);
	exception.ThrowError(exception.E0082, expr);
	expr.clear();
}
// Whether or not to add a new preprocessor definition
void prestmt_define(Expr &expr, Lexer &lexer) {
	System::Vector::push_range(expr, lexer.GetLine());
	if (lexer.Define.IsGood(expr)) lexer.Define.append(expr);
	expr.clear();
}
// Throw an error if the token is undefined
void prestmt_err_NoDefined(Exception &exception, Expr &expr) {
	exception.ThrowError(exception.E0080, expr);
	expr.clear();
}
// Throw an error if the token is unknown
void prestmt_err_unknown(Exception &exception, Expr &expr) {
	exception.ThrowError(exception.E0081, expr);
	expr.clear();
}
// If the original token is undefined, try to find its expression statement
int Parser::parser::prestmt_NoDefined(token_t &token, Expr &expr, TokenList::TokenList TokenType,
	StatementTypes::StatementTypes &StatementType, Lexer &lexer) {
	while (token.type != TokenList::ENDLINE) LexPushInExp(token, expr, lexer);

	StatementType = DetermineStatement(TokenType, expr, lexer);

	if (StatementType == StatementTypes::NoDefined) {
		prestmt_err_NoDefined(exception, expr);
		return BREAK;
	}
	if (StatementType == StatementTypes::UNKNOWN) {
		prestmt_err_unknown(exception, expr);
		return BREAK;
	}

	return !BREAK;
}
// If the instruction is validated by the expression, add it to the AST
void Parser::parser::SetStatement(StatementTypes::StatementTypes StatementType, Lexer &lexer, Lexer &LocalLexer,
	std::vector<std::string> &HeaderImported, token_t token, Expr &expr, bool IsHeader) {
	pushexpr(LEXER, expr);
	CleanExpr(expr);
	if (StatementType != StatementTypes::SingleToken)
		AppendToAst(StatementType, expr);
	expr.clear();
}
bool isEof(Lexer &lexer) {
	return (lexer.eof || lexer.peekchr() == EOF);
}

// Main function of parser: start parsing
void Parser::parser::parse
(Lexer &lexer, std::vector<std::string> &HeaderImported, std::string HeaderFilename) {
	Lexer LocalLexer;
	token_t token;
	Expr expr;
	bool IsHeader = (!HeaderFilename.empty());
	if (isEof(LEXER)) return;
	if (IsHeader) LocalLexer.start(HeaderFilename);
	if (IsHeader && !System::Vector::Contains(HeaderImported, HeaderFilename)) HeaderImported.push_back(HeaderFilename);
	
	for (;;) {

		if (LEXER.peekt().value[0] == EOF) break;
		
		token = LEXER.next();

		if (token.type == TokenList::ENDLINE) {
			expr.clear();
			continue;
		}
		if (token.type == TokenList::LEFT_BRACE) {
			AppendToAst_StartingBlock();
			expr.clear();
			continue;
		}
		if (token.type == TokenList::RIGHT_BRACE) {
			AppendToAst_EndingBlock();
			expr.clear();
			continue;
		}

		TokenList::TokenList TokenType = token.type;
		expr.push_back(token);
		StatementTypes::StatementTypes StatementType = DetermineStatement(TokenType, expr, LEXER);

		if (StatementType == StatementTypes::Nothing) break;
		if (StatementType == StatementTypes::DefineStmt) {
			prestmt_define(expr, LEXER);
			UpdateLexerDefine(lexer, LocalLexer);
			continue;
		}
		if (StatementType == StatementTypes::SingleToken) {
			prestmt_single(token, expr, LEXER);
			StatementType = DetermineStatement(TokenType, expr, LEXER);
		}
		if (StatementType == StatementTypes::NoDefined) {
			if (prestmt_NoDefined(token, expr, TokenType, StatementType, LEXER) == BREAK) break;
		}

		if (StatementType == StatementTypes::ImportStmt) {
			import(IsHeader, expr, lexer, LocalLexer, token, HeaderImported);
			continue;
		}

		if (StatementType == StatementTypes::UNKNOWN) {
			exception.ThrowError(exception.E0081, expr);
			expr.clear();
		}
		else SetStatement(StatementType, lexer, LocalLexer, HeaderImported, token, expr, IsHeader);

		if (MustBreak(LEXER)) break;
		
	}

	// End of AST's filling OR error due of statement parsing | for this file

}
