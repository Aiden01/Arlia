#include "ParserFuncs.hpp"

bool Parser::isNamespaceObject_Function(Expr expr) {
	return (expr[0].type == TokenList::NAMESPACE_CALLING
		&& expr[2].type == TokenList::DOUBLEPOINT
		&& expr[4].type == TokenList::DOT
		&& expr[6].type == TokenList::LEFT_PARENTHESIS);
}
bool Parser::isNamespace_Function(Expr expr) {
	return (expr[0].type == TokenList::NAMESPACE_CALLING
		&& expr[2].type == TokenList::DOUBLEPOINT
		&& expr[4].type == TokenList::LEFT_PARENTHESIS);
}
bool Parser::isObject_Function(Expr expr) {
	return (expr[1].type == TokenList::DOT
		&& expr[3].type == TokenList::LEFT_PARENTHESIS);
}
bool Parser::is_Function(Expr expr) {
	return (expr[1].type == TokenList::LEFT_PARENTHESIS);
}
Parser::TypeOfFuncCall Parser::TypeOfFunctionCall(Expr expr) {
	while (expr.back().type == TokenList::ENDLINE)
		if (!expr.empty()) expr.pop_back();
	if (expr.back().type != TokenList::RIGHT_PARENTHESIS || expr.size() < 3)
		return Parser::TypeOfFuncCall::UNKNOWN;
	else if (expr.size() >= 7 && Parser::isNamespaceObject_Function(expr))
		return Parser::TypeOfFuncCall::NamespaceObject_Function;
	else if (expr.size() >= 5 && Parser::isNamespace_Function(expr))
		return Parser::TypeOfFuncCall::Namespace_Function;
	else if (expr.size() >= 4 && Parser::isObject_Function(expr))
		return Parser::TypeOfFuncCall::Object_Function;
	else if (expr.size() >= 3 && Parser::is_Function(expr))
		return Parser::TypeOfFuncCall::_Function;
	return Parser::TypeOfFuncCall::UNKNOWN;
}

