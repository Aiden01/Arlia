#include "System.hpp"
#include "TokenList.h"
#include "Location.hpp"
#include "AST.hpp"

#include "FunctionDecl.hpp"

/// private

void Parser::AST::AST::Add_Function_Decl(FunctionDecl_t decl, std::vector<token_t> line) {
	AddFunction(decl, line);
}

/// public

void Parser::AST::AST::AddInstruction(Parser::AstStatementTypes StatementType, std::vector<token_t> line) {
	switch (StatementType) {
	case AstStatementTypes::FunctionDecl:
		break;
	case AstStatementTypes::RuleDecl:
		break;
	case AstStatementTypes::ObjectDecl:
		break;
	case AstStatementTypes::InstanceDecl:
		break;
	case AstStatementTypes::EnumDecl:
		break;
	case AstStatementTypes::NamespaceDecl:
		break;
	case AstStatementTypes::VariableDecl:
		break;
	case AstStatementTypes::RetDecl:
		break;
	case AstStatementTypes::ForStmt:
		break;
	case AstStatementTypes::WhileStmt:
		break;
	case AstStatementTypes::DoStmt:
		break;
	case AstStatementTypes::ReturnStmt:
		break;
	case AstStatementTypes::IfStmt:
		break;
	case AstStatementTypes::ElifStmt:
		break;
	case AstStatementTypes::ElseStmt:
		break;
	case AstStatementTypes::ExternStmt:
		break;
	case AstStatementTypes::MatchStmt:
		break;
	case AstStatementTypes::CaseStmt:
		break;
	case AstStatementTypes::DefaultStmt:
		break;
	case AstStatementTypes::ContinueStmt:
		break;
	case AstStatementTypes::ThrowStmt:
		break;
	case AstStatementTypes::TryStmt:
		break;
	case AstStatementTypes::CatchStmt:
		break;
	case AstStatementTypes::SetStmt:
		break;
	case AstStatementTypes::DeleteStmt:
		break;
	case AstStatementTypes::NewStmt:
		break;
	case AstStatementTypes::FreeStmt:
		break;
	case AstStatementTypes::PublicStmt:
		break;
	case AstStatementTypes::PrivateStmt:
		break;
	case AstStatementTypes::StaticStmt:
		break;
	case AstStatementTypes::UponStmt:
		break;
	case AstStatementTypes::FunctionCalling:
		break;
	default:
		break;
	}
}
void Parser::AST::AST::PrintAst() {
	for (Instruction node : this->AstProgramObj) {
		// ...
	}
}
