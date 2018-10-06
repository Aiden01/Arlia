#include "Maker.hpp"
#include "ParserFuncs.hpp"

void Parser::Maker::make(StatementTypes::StatementTypes StatementType, Expr expr) {
	switch (StatementType) {
	case StatementTypes::FunctionDecl:
		make_FuncDecl(expr);
		break;
	case StatementTypes::TypeDecl:
		make_TypeDecl(expr);
		break;
	case StatementTypes::VariableDecl:
		make_VarDecl(expr);
		break;
	case StatementTypes::LetDecl:
		make_LetDecl(expr);
		break;
	case StatementTypes::ReturnStmt:
		make_ReturnStmt(expr);
		break;
	case StatementTypes::ForStmt:
		make_ForStmt(expr);
		break;
	case StatementTypes::WhileStmt:
		make_WhileStmt(expr);
		break;
	case StatementTypes::IfStmt:
		make_IfStmt(expr);
		break;
	case StatementTypes::ElifStmt:
		make_ElifStmt(expr);
		break;
	case StatementTypes::MatchStmt:
		make_MatchStmt(expr);
		break;
	case StatementTypes::ThrowStmt:
		make_ThrowStmt(expr);
		break;
	case StatementTypes::CatchStmt:
		make_CatchStmt(expr);
		break;
	case StatementTypes::DeleteStmt:
		make_DeleteStmt(expr);
		break;
	case StatementTypes::FreeStmt:
		make_FreeStmt(expr);
		break;
	case StatementTypes::ElseStmt:
		make_ElseStmt(expr);
		break;
	case StatementTypes::NamespaceDecl:
		make_NameSpaceDecl(expr);
		break;
	case StatementTypes::ContinueStmt:
		make_ContinueStmt(expr);
		break;
	case StatementTypes::TryStmt:
		make_TryStmt(expr);
		break;
	case StatementTypes::ExterneStmt:
		make_ExterneStmt(expr);
		break;
	case StatementTypes::FunctionCalling:
		make_FunctionCall(expr);
		break;
	case StatementTypes::IdentifierEdit:
		make_VarEdit(expr);
		break;
	default:
		break;
	}
}

void Parser::Maker::make_VarDecl(Expr expr) {

}
void Parser::Maker::make_FuncDecl(Expr expr) {

}
void Parser::Maker::make_TypeDecl(Expr expr) {

}
void Parser::Maker::make_LetDecl(Expr expr) {

}
void Parser::Maker::make_ReturnStmt(Expr expr) {

}
void Parser::Maker::make_ForStmt(Expr expr) {

}
void Parser::Maker::make_WhileStmt(Expr expr) {

}
void Parser::Maker::make_MatchStmt(Expr expr) {

}
void Parser::Maker::make_IfStmt(Expr expr) {

}
void Parser::Maker::make_ElifStmt(Expr expr) {

}
void Parser::Maker::make_ElseStmt(Expr expr) {

}
void Parser::Maker::make_ThrowStmt(Expr expr) {

}
void Parser::Maker::make_CatchStmt(Expr expr) {

}
void Parser::Maker::make_DeleteStmt(Expr expr) {

}
void Parser::Maker::make_FreeStmt(Expr expr) {

}
void Parser::Maker::make_NameSpaceDecl(Expr expr) {

}
void Parser::Maker::make_ContinueStmt(Expr expr) {

}
void Parser::Maker::make_TryStmt(Expr expr) {

}
void Parser::Maker::make_ExterneStmt(Expr expr) {

}
void Parser::Maker::make_FunctionCall(Expr expr) {

}
void Parser::Maker::make_VarEdit(Expr expr) {

}

void Parser::Maker::startBlock() {

}
void Parser::Maker::endBlock() {

}

