#include "Parser.hpp"
#include "Expression.hpp"

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



enum TypeOfFuncCall {
	NamespaceObject_Function, // @namespace:object.function()
	Namespace_Function,		  // @namespace:function()
	Object_Function,		  // object.function()
	_Function,				  // function()
	UNKNOWN
};
#include <chrono> 
TypeOfFuncCall TypeOfFunctionCall(Expr expr) {
	// @namespace:object.function()  [6] -> 7
	// @namespace.function()		 [4] -> 5
	// object.function()			 [3] -> 4
	// function()					 [1] -> 3

	if (expr.back().type != TokenList::RIGHT_PARENTHESIS || expr.size() < 3) return TypeOfFuncCall::UNKNOWN;
	if (expr.size() >= 7) {
		if (expr[0].type == TokenList::NAMESPACE_CALLING)
			if (expr[2].type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL)
				if (expr[6].type == TokenList::LEFT_PARENTHESIS)
					return TypeOfFuncCall::NamespaceObject_Function;
	}
	if (expr.size() >= 5) {
		if (expr[0].type == TokenList::NAMESPACE_CALLING)
			if (expr[2].type == TokenList::DOT)
				if (expr[4].type == TokenList::LEFT_PARENTHESIS)
					return TypeOfFuncCall::Namespace_Function;
	}
	if (expr.size() >= 4) {
		if (expr[1].type == TokenList::DOT)
			if (expr[3].type == TokenList::LEFT_PARENTHESIS)
				return TypeOfFuncCall::Object_Function;
	}
	if (expr.size() >= 3) {
		if (expr[1].type == TokenList::LEFT_PARENTHESIS) return TypeOfFuncCall::_Function;
	}
	return TypeOfFuncCall::UNKNOWN;
}
bool IsFunction(Expr expr) {
	TypeOfFuncCall TypeOfFunc = TypeOfFunctionCall(expr);
	return (
		TypeOfFunc == TypeOfFuncCall::NamespaceObject_Function
		|| TypeOfFunc == TypeOfFuncCall::Namespace_Function
		|| TypeOfFunc == TypeOfFuncCall::Object_Function
		|| TypeOfFunc == TypeOfFuncCall::_Function
		&& TypeOfFunc != TypeOfFuncCall::UNKNOWN
		);
}
bool IsVariableEditingSymbol(token_t token) {
	return
		(
			TokenList::IsSymbol(token.value)
			&& token.type != TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL
			&& token.type != TokenList::WITH
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
		);
}
bool IsEnumSuite(Expr expr) {
	for (size_t i = 0; i < expr.size(); ++i) {
		if (i % 2 == 0 && i > 1 || i == 0 && i < expr.size()) if (expr[i].type != TokenList::IDENTIFIER) return false;
		if (i % 2 != 0 && i > 1 && i < expr.size()) if (expr[i].type != TokenList::COMMA) return false;
		if (i == expr.size()) if (expr[i].type != TokenList::IDENTIFIER) return false;
	}
	return true;
}
Parser::parser::StatementTypes Parser::parser::DetermineStatement(TokenList::TokenList TokenType, Expr expr, Lexer &lexer) {
	switch (TokenType) {
	case TokenList::NOTHING: return StatementTypes::Nothing;			// -1
	case TokenList::VAR: return StatementTypes::VariableDecl;			// var
	case TokenList::FUNC: return StatementTypes::FunctionDecl;			// func
	case TokenList::RET: return StatementTypes::RetDecl;				// ret
	case TokenList::IMPLEMENT: return StatementTypes::ImplementStmt;	// implement
	case TokenList::ENUM: return StatementTypes::EnumDecl;				// enum
	case TokenList::NAMESPACE: return StatementTypes::NamespaceDecl;	// namespace
	case TokenList::WHILE: return StatementTypes::WhileStmt;			// while
	case TokenList::FOR: return StatementTypes::ForStmt;				// for
	case TokenList::STRUCTURE: return StatementTypes::StructureDecl;	// structure
	case TokenList::RETURN: return StatementTypes::ReturnStmt;			// return
	case TokenList::CONTINUE: return StatementTypes::ContinueStmt;		// continue
	case TokenList::SET: return StatementTypes::SetStmt;				// set
	case TokenList::IF: return StatementTypes::IfStmt;					// if
	case TokenList::ELIF: return StatementTypes::ElifStmt;				// elif
	case TokenList::ELSE: return StatementTypes::ElseStmt;				// else
	case TokenList::MATCH: return StatementTypes::MatchStmt;			// match
	case TokenList::CASE: return StatementTypes::CaseStmt;				// case
	case TokenList::DEFAULT: return StatementTypes::DefaultStmt;		// default
	case TokenList::EXTERNE: return StatementTypes::ExterneStmt;		// externe
	case TokenList::THROW: return StatementTypes::ThrowStmt;			// throw
	case TokenList::TRY: return StatementTypes::TryStmt;				// try
	case TokenList::CATCH: return StatementTypes::CatchStmt;			// catch
	case TokenList::_DELETE: return StatementTypes::DeleteStmt;			// delete
	case TokenList::FREE: return StatementTypes::FreeStmt;				// free
	case TokenList::DEFINE: return StatementTypes::DefineStmt;			// define
	case TokenList::IMPORT: return StatementTypes::ImportStmt;			// import
	case TokenList::PROC: return StatementTypes::ProcStmt;				// proc
	case TokenList::GOTO: return StatementTypes::GotoStmt;				// implement

	case TokenList::PRIVATE: return StatementTypes::PrivateMember;		// private
	case TokenList::PUBLIC: return StatementTypes::PublicMember;		// public
	case TokenList::STATIC: return StatementTypes::StaticMember;		// static
	case TokenList::UPON: return StatementTypes::UponMember;			// upon

	default:
		if (expr.empty()) return StatementTypes::NoDefined;
		if (expr[0].type == TokenList::INSTANCE) {
			if (expr.size() == 1) return StatementTypes::InstanceDecl;
			if (expr[1].type == TokenList::RET) return StatementTypes::RuleDecl;
			else if (expr[1].type == TokenList::STRUCTURE) return StatementTypes::ObjectDecl;
			else return StatementTypes::InstanceDecl;
		}
		if (expr.size() > 2 && expr.back().type == TokenList::ENDLINE) expr.pop_back(); // erase potential ';' for analyse
		if (expr.size() >= 3) if (IsFunction(expr)) return StatementTypes::FunctionCalling;
		if (expr.size() == 1) return StatementTypes::SingleToken;
		if (IsEnumSuite(expr)) return StatementTypes::EnumSuite;
		if (expr[0].type == TokenList::IDENTIFIER && IsVariableEditingSymbol(expr[1])) return StatementTypes::IdentifierEdit;
		else if (expr[1].type == TokenList::IDENTIFIER && IsVariableEditingSymbol(expr[0])) return StatementTypes::IdentifierEdit;
		return StatementTypes::UNKNOWN;
	}
}
// Checks if the expression that follows the statement ends with a semicolon or a right parenthesis or left brace.
Parser::parser::TypeOfEndExpr Parser::parser::StatementEndExpr(Parser::parser::StatementTypes StatementType) {
	// Right Parenthesis ending expression -> ')'
	if (StatementType == StatementTypes::ForStmt
		|| StatementType == StatementTypes::WhileStmt
		|| StatementType == StatementTypes::Each
		|| StatementType == StatementTypes::IfStmt
		|| StatementType == StatementTypes::ElifStmt
		|| StatementType == StatementTypes::MatchStmt
		|| StatementType == StatementTypes::CaseStmt
		|| StatementType == StatementTypes::CatchStmt
		|| StatementType == StatementTypes::ImplementStmt
		) return TypeOfEndExpr::RightParenthesis;
	// Left Brace ending expression -> '{'
	if (StatementType == StatementTypes::FunctionDecl
		|| StatementType == StatementTypes::StructureDecl
		|| StatementType == StatementTypes::ObjectDecl
		|| StatementType == StatementTypes::ElseStmt
		|| StatementType == StatementTypes::DefaultStmt
		|| StatementType == StatementTypes::EnumDecl
		|| StatementType == StatementTypes::TryStmt
		|| StatementType == StatementTypes::SetStmt
		) return TypeOfEndExpr::LeftBrace;
	if (StatementType == StatementTypes::EnumSuite)
		return TypeOfEndExpr::RightBrace;
	// End line expression -> ';'
	return TypeOfEndExpr::EndLine;
}
void Parser::parser::UpdateLexerDefine(Lexer &lexer, Lexer &LocalLexer) {
	LocalLexer.Define.AppendRange(lexer.Define);
	lexer.Define = LocalLexer.Define;
}
void Parser::parser::AppendToAst(Parser::parser::StatementTypes StatementType, Parser::parser::TypeOfEndExpr EnfOfExprType, Expr expr) {
	for (auto token : expr)
		std::cout << token.value << std::endl;
}
void Parser::parser::AppendToAst_StartingBlock() {
	//std::cout << "START" << std::endl;
}
void Parser::parser::AppendToAst_EndingBlock() {
	//std::cout << "END" << std::endl;
}

#define LEXER		(IsHeader ? LocalLexer : lexer)
#define BREAK	    1

bool MustBreak(Lexer &lexer) {
	return (lexer.eof || !lexer.CanContinue && !lexer.peekchr() == EOF);
}
// Modify the expression sent in relation to the type of end of line as well as the type of exercise (local or general)
void Parser::parser::SetExprViaEndLineTypeAndLexer(Expr &expr, TypeOfEndExpr &TypeOfEndExpr, Lexer &lexer) {
	/// ! Si erreur d'accolades !
	if (lexer.eof) return;
	if (TypeOfEndExpr == TypeOfEndExpr::EndLine)
		System::Vector::push_range(expr, lexer.GetLine());
	if (TypeOfEndExpr == TypeOfEndExpr::LeftBrace)
		System::Vector::push_range(expr, lexer.GetUntil(TokenList::LEFT_BRACE, false));
	if (TypeOfEndExpr == TypeOfEndExpr::RightParenthesis)
		System::Vector::push_range(expr, lexer.GetUntil(TokenList::RIGHT_PARENTHESIS, true));
	if (TypeOfEndExpr == TypeOfEndExpr::RightBrace) return; // Nothing to do, since the expression has already been treated
}
void Parser::parser::import(bool IsHeader, Expr &expr, Lexer &lexer, Lexer &LocalLexer, token_t token, std::vector<std::string> &HeaderImported) {
	System::Vector::push_range(expr, LEXER.GetLine());
	if (CanImport(expr, HeaderImported)) parse(LocalLexer, HeaderImported, GetImportFilename(expr));
	else ImportFailed(expr, token.position.filename, exception, lexer.CanContinue, lexer.StopAferFirstError);
	UpdateLexerDefine(lexer, LocalLexer);
	expr.clear();
}

/// Nombres !! Lexer !!!

// Gets the new token from the lexer, and adds it to the current expression
void LexPushInExp(token_t &token, Expr &expr, Lexer &lexer) {
	token = lexer.next();
	expr.push_back(token);
}
// Gets all the expression to its end
void prestmt_single_isin_true(token_t &inst_t, Expr &expr, Lexer &lexer) {
	do {
		LexPushInExp(inst_t, expr, lexer);
	} while (inst_t.type != TokenList::ENDLINE && !lexer.eof);
}
// Builds the current expression if the original token is unidentified (single)
void prestmt_single_isin(token_t &token, Lexer &lexer, Expr &expr) {
	while (token.type != TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL || token.type != TokenList::DOT) {
		LexPushInExp(token, expr, lexer);
		if (token.type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) prestmt_single_isin_true(token, expr, lexer);
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
// Gets the new token from the lexer, and adds it to the current expression for the 'instance' token
void prestmt_instance(Expr &expr, Lexer &lexer) {
	token_t inst_t;
	LexPushInExp(inst_t, expr, lexer);
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
int Parser::parser::prestmt_NoDefined(token_t &token, Expr &expr, TokenList::TokenList TokenType, StatementTypes &StatementType, Lexer &lexer) {
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
void Parser::parser::SetStatement(StatementTypes StatementType, Lexer &lexer, Lexer &LocalLexer, std::vector<std::string> &HeaderImported, token_t token, Expr &expr, bool IsHeader) {
	TypeOfEndExpr TypeOfEndExpr = StatementEndExpr(StatementType);
	if (StatementType == StatementTypes::ImportStmt) {
		import(IsHeader, expr, lexer, LocalLexer, token, HeaderImported);
		return;
	}
	SetExprViaEndLineTypeAndLexer(expr, TypeOfEndExpr, lexer);
	AppendToAst(StatementType, TypeOfEndExpr, expr);
	expr.clear();
}


void Parser::parser::parse
(Lexer &lexer, std::vector<std::string> &HeaderImported, std::string HeaderFilename) {
	if (lexer.eof) return;
	Lexer LocalLexer;
	token_t token;
	Expr expr;
	bool IsHeader = (!HeaderFilename.empty());

	if (IsHeader) LocalLexer.start(HeaderFilename);
	if (IsHeader && !System::Vector::Contains(HeaderImported, HeaderFilename)) HeaderImported.push_back(HeaderFilename);

	for (;;) {
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
		StatementTypes StatementType = DetermineStatement(TokenType, expr, LEXER);

		if (StatementType == StatementTypes::Nothing) break;
		if (StatementType == StatementTypes::DefineStmt) {
			prestmt_define(expr, LEXER);
			UpdateLexerDefine(lexer, LocalLexer);
			continue;
		}
		if (StatementType == StatementTypes::InstanceDecl) {
			prestmt_instance(expr, LEXER);
			StatementType = DetermineStatement(TokenType, expr, LEXER);
		}
		if (StatementType == StatementTypes::SingleToken) {
			prestmt_single(token, expr, LEXER);
			StatementType = DetermineStatement(TokenType, expr, LEXER);
		}
		if (StatementType == StatementTypes::NoDefined) {
			if (prestmt_NoDefined(token, expr, TokenType, StatementType, LEXER) == BREAK) break;
		}

		if (StatementType == StatementTypes::UNKNOWN) {
			exception.ThrowError(exception.E0081, expr);
			expr.clear();
		}
		else SetStatement(StatementType, lexer, LocalLexer, HeaderImported, token, expr, IsHeader);

		if (MustBreak(LEXER)) break;;
		
	}

	// End of AST's filling OR error due of statement parsing | for this file

}
