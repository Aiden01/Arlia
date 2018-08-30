#include "DefineStmt.hpp"

/*
	define name value;

	expr[0] = define
	expr[1] = name
	expr[2...k-1] = value
	expr[k] = ;
*/

bool Parser::DefineStatement::IsGood(Expr expr) {
	if (IsDefined(expr[1].value)) {
		exception.ThrowError(exception.E0008, expr[1]);
		return false;
	}
	if (expr[1].type != TokenList::IDENTIFIER) {
		exception.ThrowError(exception.E0028, expr[1]);
		return false;
	}
	if (expr.back().type != TokenList::ENDLINE) {
		exception.ThrowError(exception.E0016, expr);
		return false;
	}
	return true;
}
bool Parser::DefineStatement::IsDefined(std::string name) {
	return System::Map::Contains(list, name);
}
void Parser::DefineStatement::append(Expr expr) {
	std::string name = expr[1].value;
	expr.erase(expr.begin(), expr.begin() + 2);
	expr.pop_back();
	list.insert(std::pair<std::string, Expr>(name, expr));
}
void Parser::DefineStatement::AppendRange(DefineStatement Define) {
	list.insert(Define.list.begin(), Define.list.end());
}
Expr Parser::DefineStatement::get(std::string name) {
	return list[name];
}
