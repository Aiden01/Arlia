#pragma once
#include "SyntaxTree.hpp"

namespace Parser {
	class Visitor {
	public:
		virtual ~Visitor() { }
		virtual void Visit(Parser::SyntaxTree::NamespaceStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::Block*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ReturnStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::VarStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::LetStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::WhileStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::IfStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ElifStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ElseStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ForEachStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ForStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ForStepStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::FunctionStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::AssignmentStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::TypeStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::BinaryExpression*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::UnaryExpression*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::FunctionBody*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ParamList*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::NameList*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::NormalFuncCall*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::MemberFuncCall*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::FuncCallArgs*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::ExterneStatement*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::TryCatchExpression*, void*) = 0;
		virtual void Visit(Parser::SyntaxTree::VariableEdit*, void*) = 0;
	};
}
