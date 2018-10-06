#include "visitor.hpp"

void Parser::NamespaceDeclaration::accept(Parser::Visitor &v)	{ v.visit(this); }
void Parser::TypeDeclaration::accept(Parser::Visitor &v)		{ v.visit(this); }
void Parser::VariableDeclaration::accept(Parser::Visitor &v)	{ v.visit(this); }
void Parser::IfStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::ForLoopStatement::accept(Parser::Visitor &v)		{ v.visit(this); }
void Parser::MatchStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::ExterneStatement::accept(Parser::Visitor &v)		{ v.visit(this); }
void Parser::WhileStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::ReturnStatement::accept(Parser::Visitor &v)		{ v.visit(this); }
void Parser::ElifStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::ElseStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::TryStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::ThrowStatement::accept(Parser::Visitor &v)			{ v.visit(this); }
void Parser::CatchStatement::accept(Parser::Visitor &v)			{ v.visit(this); }

