#include "tast.hpp"

enum TypeOfFuncCall {
	NamespaceObject_Function, // @namespace:object.function()
	Namespace_Function,		  // @namespace:function()
	Object_Function,		  // object.function()
	_Function,				  // function()
	UNKNOWN
};

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
	return (
		TypeOfFunctionCall(expr) == TypeOfFuncCall::NamespaceObject_Function
		|| TypeOfFunctionCall(expr) == TypeOfFuncCall::Namespace_Function
		|| TypeOfFunctionCall(expr) == TypeOfFuncCall::Object_Function 
		|| TypeOfFunctionCall(expr) == TypeOfFuncCall::_Function
		&& TypeOfFunctionCall(expr) != TypeOfFuncCall::UNKNOWN
		);
}
tast::Ast::AstStatementTypes tast::Ast::DetermineStatement(TokenList::TokenList TokenType, Expr expr) {
	switch (TokenType) {
	case TokenList::VAR: return AstStatementTypes::VariableDecl;
	case TokenList::FUNC: return AstStatementTypes::FunctionDecl;
	case TokenList::RET: return AstStatementTypes::RetDecl;
	case TokenList::IMPLEMENT: return AstStatementTypes::Implement;
	case TokenList::ENUM: return AstStatementTypes::EnumDecl;
	case TokenList::NAMESPACE: return AstStatementTypes::NamespaceDecl;
	case TokenList::WHILE: return AstStatementTypes::WhileStmt;
	case TokenList::FOR: return AstStatementTypes::ForStmt;
	case TokenList::STRUCTURE: return AstStatementTypes::StructureDecl;
	case TokenList::RETURN: return AstStatementTypes::ReturnStmt;
	case TokenList::CONTINUE: return AstStatementTypes::ContinueStmt;
	case TokenList::SET: return AstStatementTypes::SetStmt;
	case TokenList::IF: return AstStatementTypes::IfStmt;
	case TokenList::ELIF: return AstStatementTypes::ElifStmt;
	case TokenList::ELSE: return AstStatementTypes::ElseStmt;
	case TokenList::MATCH: return AstStatementTypes::MatchStmt;
	case TokenList::CASE: return AstStatementTypes::CaseStmt;
	case TokenList::DEFAULT: return AstStatementTypes::DefaultStmt;
	case TokenList::EXTERN: return AstStatementTypes::ExternStmt;
	case TokenList::THROW: return AstStatementTypes::ThrowStmt;
	case TokenList::TRY: return AstStatementTypes::TryStmt;
	case TokenList::CATCH: return AstStatementTypes::CatchStmt;
	case TokenList::EACH: return AstStatementTypes::Each;
	case TokenList::NEW: return AstStatementTypes::NewStmt;
	case TokenList::_DELETE: return AstStatementTypes::DeleteStmt;
	case TokenList::FREE: return AstStatementTypes::FreeStmt;
	default:
		if (expr[0].type == TokenList::INSTANCE) {
			if (expr[1].type == TokenList::RET) return AstStatementTypes::RuleDecl;
			else if (expr[1].type == TokenList::STRUCTURE) return AstStatementTypes::ObjectDecl;
			else return AstStatementTypes::InstanceDecl;
		}
		if (IsFunction(expr)) return AstStatementTypes::FunctionCalling;
		if (expr[0].type == TokenList::IDENTIFIER
			|| (expr[1].type == TokenList::IDENTIFIER)
			&& TokenList::IsSymbol(expr[0].value))
			return AstStatementTypes::IdentifierEdit;
		return AstStatementTypes::UNKNOWN;
	}
}
bool tast::Ast::IsNode(token_t token) {
	return (token.type > NodeTypes::START && token.type < NodeTypes::END
		|| token.type == NodeTypes::Is
		|| token.type == NodeTypes::Isnt
		|| token.type == NodeTypes::To
		|| token.type == NodeTypes::In);
}
bool tast::Ast::ContainNodes(Expr expr) {
	for (token_t token : expr)
		if (IsNode(token)) return true;
	return false;
}
tast::Ast::NodeTypes tast::Ast::DetermineFirstNode(Expr expr) {
	for (token_t token : expr) if (IsNode(token))
		return static_cast<NodeTypes>(token.type);
	return NodeTypes::UnknownNode;
}
void tast::Ast::append(Expr expr) {
	AstStatementTypes StatementType = DetermineStatement(expr[0].type, expr);
	NodeTypes NodeType = NodeTypes::START; // start is not a node, but an initiator --> no nodes
	if (ContainNodes(expr)) NodeType = DetermineFirstNode(expr);
	if (StatementType == AstStatementTypes::UNKNOWN) {
		exception.ThrowError(exception.E0079, expr[0]);
		return;
	}
}
void tast::Ast::add(AstStatementTypes StatementType, NodeTypes NodeType) {

}
