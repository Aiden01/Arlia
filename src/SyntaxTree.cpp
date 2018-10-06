#include "SyntaxTree.hpp"
#include "visitor.hpp"


#define ACCEPT_VISITOR_IMPL(class_name) void Parser::SyntaxTree::class_name::accept(Parser::Visitor *v, void *data) \
	{ v->Visit(this, data); }


ACCEPT_VISITOR_IMPL(NamespaceStatement)
ACCEPT_VISITOR_IMPL(Block)
ACCEPT_VISITOR_IMPL(ReturnStatement)
ACCEPT_VISITOR_IMPL(VarStatement)
ACCEPT_VISITOR_IMPL(LetStatement)
ACCEPT_VISITOR_IMPL(WhileStatement)
ACCEPT_VISITOR_IMPL(IfStatement)
ACCEPT_VISITOR_IMPL(ElifStatement)
ACCEPT_VISITOR_IMPL(ElseStatement)
ACCEPT_VISITOR_IMPL(ForEachStatement)
ACCEPT_VISITOR_IMPL(ForStatement)
ACCEPT_VISITOR_IMPL(ForStepStatement)
ACCEPT_VISITOR_IMPL(FunctionStatement)
ACCEPT_VISITOR_IMPL(TypeStatement)
ACCEPT_VISITOR_IMPL(BinaryExpression)
ACCEPT_VISITOR_IMPL(UnaryExpression)
ACCEPT_VISITOR_IMPL(FunctionBody)
ACCEPT_VISITOR_IMPL(ParamList)
ACCEPT_VISITOR_IMPL(NameList)
ACCEPT_VISITOR_IMPL(NormalFuncCall)
ACCEPT_VISITOR_IMPL(MemberFuncCall)
ACCEPT_VISITOR_IMPL(FuncCallArgs)
ACCEPT_VISITOR_IMPL(ExterneStatement)
ACCEPT_VISITOR_IMPL(TryCatchExpression)
ACCEPT_VISITOR_IMPL(AssignmentStatement)
ACCEPT_VISITOR_IMPL(VariableEdit)
