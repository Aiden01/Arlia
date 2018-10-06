#pragma once
#include "System.hpp"

namespace Parser {
#pragma region Statements
	class NamespaceDeclaration;
	class TypeDeclaration;
	class VariableDeclaration;
	class IfStatement;
	class ForLoopStatement;
	class MatchStatement;
	class ExterneStatement;
	class WhileStatement;
	class ReturnStatement;
	class ElifStatement;
	class ElseStatement;
	class TryStatement;
	class ThrowStatement;
	class CatchStatement;
#pragma endregion
#pragma region NodeTypes
	class Program;
	class RegularStatement;
	class TopLevelStatement;
	class Block;
	class Parameter;
	class Argument;
	class Statement;
	class Pattern;
	class MatchCase;
	class Expression;
	class BinaryOperator;
	class UnaryOperator;
	class TypeExpression;
#pragma endregion

	class Statement {
	public:
		virtual void accept(class Visitor &v) = 0;
	};
	class TopLevelStatement {

	};
	class RegularStatement : TopLevelStatement {

	};

	class NamespaceDeclaration : public Statement {
		void accept(Visitor &v);
	};
	class TypeDeclaration : public Statement {
		void accept(Visitor &v);
	};
	class VariableDeclaration : public Statement {
		void accept(Visitor &v);
	};
	class IfStatement : public Statement {
		void accept(Visitor &v);
	};
	class ForLoopStatement : public Statement {
		void accept(Visitor &v);
	};
	class MatchStatement : public Statement {
		void accept(Visitor &v);
	};
	class ExterneStatement : public Statement {
		void accept(Visitor &v);
	};
	class WhileStatement : public Statement {
		void accept(Visitor &v);
	};
	class ReturnStatement : public Statement {
		void accept(Visitor &v);
	};
	class ElifStatement : public Statement {
		void accept(Visitor &v);
	};
	class ElseStatement : public Statement {
		void accept(Visitor &v);
	};
	class TryStatement : public Statement {
		void accept(Visitor &v);
	};
	class ThrowStatement : public Statement {
		void accept(Visitor &v);
	};
	class CatchStatement : public Statement {
		void accept(Visitor &v);
	};

	class Visitor {
	public:
		virtual void visit(NamespaceDeclaration *stmt) = 0;
		virtual void visit(TypeDeclaration *stmt) = 0;
		virtual void visit(VariableDeclaration *stmt) = 0;
		virtual void visit(IfStatement *stmt) = 0;
		virtual void visit(ForLoopStatement *stmt) = 0;
		virtual void visit(MatchStatement *stmt) = 0;
		virtual void visit(ExterneStatement *stmt) = 0;
		virtual void visit(WhileStatement *stmt) = 0;
		virtual void visit(ReturnStatement *stmt) = 0;
		virtual void visit(ElifStatement *stmt) = 0;
		virtual void visit(ElseStatement *stmt) = 0;
		virtual void visit(TryStatement *stmt) = 0;
		virtual void visit(ThrowStatement *stmt) = 0;
		virtual void visit(CatchStatement *stmt) = 0;
	};

}






