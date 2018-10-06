#pragma once
#include "System.hpp"
#include "TokenList.h"

namespace Parser {
	class Visitor;
	namespace SyntaxTree {

#define ACCEPT_VISITOR virtual void accept(class Parser::Visitor *v, void *data) = 0;
#define ACCEPT_VISITOR_DECL virtual void accept(class Parser::Visitor *v, void *data)
#define AST_uptr std::unique_ptr<SyntaxTree>
#define MOVE std::move

		enum LexScope {
			Toplevel,
			Local,
			Unknown
		};
		enum TypeMemberScope {
			Private,
			Public,
			External
		};

		class SyntaxTree {
		public:
			virtual ~SyntaxTree() {}
			ACCEPT_VISITOR;
		};

		class Statement : public SyntaxTree {
			ACCEPT_VISITOR_DECL;
		};
		class Expression : public SyntaxTree {
			ACCEPT_VISITOR_DECL;
		};

		class NamespaceStatement : public SyntaxTree {
		public:
			AST_uptr block;
			token_t namespaceToken;
			NamespaceStatement(AST_uptr block,
				const token_t namespaceToken)
				: block(MOVE(block)),
				namespaceToken(namespaceToken) {}
			ACCEPT_VISITOR_DECL;
		};
		class Block : public SyntaxTree {
		public:
			std::vector<AST_uptr> statements;
			AST_uptr return_stmt;
			Block() {}
			ACCEPT_VISITOR_DECL;
		};
		class ReturnStatement : public Statement {
		public:
			AST_uptr exp_list;
			size_t line;
			size_t exp_value_count;
			LexScope lexScope;
			explicit ReturnStatement(size_t line)
				: line(line),
				exp_value_count(0),
				lexScope(LexScope::Local) {}
			ACCEPT_VISITOR_DECL;
		};
		class WhileStatement : public Statement {
		public:
			AST_uptr exp;
			AST_uptr block;
			size_t firstLine, lastLine;
			LexScope lexScope;
			WhileStatement(AST_uptr exp,
				AST_uptr block,
				size_t firstLine,
				size_t lastLine,
				LexScope lexScope)
				: exp(MOVE(exp)),
				block(MOVE(block)),
				firstLine(firstLine),
				lastLine(lastLine),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class VarStatement : public Statement {
			AST_uptr varName;
			AST_uptr type;
			AST_uptr valueExpression;
			LexScope lexScope;
			TypeMemberScope typeMemberScope;
			VarStatement(AST_uptr varName,
				AST_uptr type,
				AST_uptr valueExpression,
				LexScope lexScope,
				TypeMemberScope typeMemberScope)
				: varName(MOVE(varName)),
				type(MOVE(type)),
				valueExpression(MOVE(valueExpression)),
				lexScope(lexScope),
				typeMemberScope(typeMemberScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class LetStatement : public Statement {
			AST_uptr letName;
			AST_uptr type;
			AST_uptr args;
			AST_uptr valueExpression;
			LexScope lexScope;
			TypeMemberScope typeMemberScope;
			LetStatement(AST_uptr letName,
				AST_uptr type,
				AST_uptr valueExpression,
				AST_uptr args,
				LexScope lexScope,
				TypeMemberScope typeMemberScope)
				: letName(MOVE(letName)),
				type(MOVE(type)),
				valueExpression(MOVE(valueExpression)),
				args(MOVE(args)),
				lexScope(lexScope),
				typeMemberScope(typeMemberScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class IfStatement : public Statement {
		public:
			AST_uptr exp;
			AST_uptr trueBranch;
			AST_uptr falseBranch;
			size_t line, endLineBlock;
			LexScope lexScope;
			IfStatement(AST_uptr exp,
				AST_uptr trueBranch,
				AST_uptr falseBranch,
				size_t line, size_t endLineBlock,
				LexScope lexScope)
				: exp(MOVE(exp)),
				trueBranch(MOVE(trueBranch)),
				falseBranch(MOVE(falseBranch)),
				line(line), endLineBlock(endLineBlock),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class ElifStatement : public Statement {
		public:
			AST_uptr exp;
			AST_uptr trueBranch;
			AST_uptr falseBranch;
			size_t line, endLineBlock;
			LexScope lexScope;
			ElifStatement(AST_uptr exp,
				AST_uptr trueBranch,
				AST_uptr falseBranch,
				size_t line, size_t endLineBlock,
				LexScope lexScope)
				: exp(MOVE(exp)),
				trueBranch(MOVE(trueBranch)),
				falseBranch(MOVE(falseBranch)),
				line(line), endLineBlock(endLineBlock),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class ElseStatement : public Statement {
		public:
			AST_uptr block;
			LexScope lexScope;
			ElseStatement(AST_uptr block, LexScope lexScope)
				: block(MOVE(block)), lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class ForStatement : public SyntaxTree {
			location_t &tokenDetail;
			AST_uptr fromExp; // i = 0
			AST_uptr UntilExp; // x + y
			AST_uptr block;
			LexScope lexScope;
			ForStatement(AST_uptr fromExp,
				AST_uptr UntilExp,
				AST_uptr block,
				LexScope lexScope)
				: tokenDetail(tokenDetail),
				fromExp(MOVE(fromExp)),
				UntilExp(MOVE(UntilExp)),
				block(MOVE(block)),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class ForStepStatement : public Statement {
			AST_uptr fromExp; // i = 0
			AST_uptr UntilExp; // x + y
			AST_uptr StepExp; // step z
			AST_uptr block;
			LexScope lexScope;
			ForStepStatement(AST_uptr fromExp,
				AST_uptr UntilExp,
				AST_uptr StepExp,
				AST_uptr block,
				LexScope lexScope)
				: fromExp(MOVE(fromExp)),
				UntilExp(MOVE(UntilExp)),
				StepExp(MOVE(StepExp)),
				block(MOVE(block)),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class ForEachStatement : public Statement {
			AST_uptr fromExp; // i
			AST_uptr InExp; // y
			AST_uptr block;
			LexScope lexScope;
			ForEachStatement(AST_uptr fromExp,
				AST_uptr InExp,
				AST_uptr block,
				LexScope lexScope)
				: fromExp(MOVE(fromExp)),
				InExp(MOVE(InExp)),
				block(MOVE(block)),
				lexScope(lexScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class FunctionStatement : public Statement {
			AST_uptr funcName;
			AST_uptr funcBody;
			TypeMemberScope typeMemberScope;
			FunctionStatement(AST_uptr funcName,
				AST_uptr funcBody,
				LexScope lexScope,
				TypeMemberScope typeMemberScope)
				: funcName(MOVE(funcName)),
				funcBody(MOVE(funcBody)),
				typeMemberScope(typeMemberScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class AssignmentStatement : public SyntaxTree {
		public:
			AST_uptr varList;
			AST_uptr expList;
			size_t line;
			size_t varCount;
			AssignmentStatement(AST_uptr varList,
				AST_uptr expList,
				size_t startLine)
				: varList(MOVE(varList)),
				expList(MOVE(expList)),
				line(MOVE(startLine)),
				varCount(0) {}
			ACCEPT_VISITOR_DECL;
		};
		class TypeStatement : public Statement {
			AST_uptr typeName;
			AST_uptr constructor;
			AST_uptr block;
			LexScope lexScope;
			TypeMemberScope typeMemberScope;
			TypeStatement(AST_uptr typeName,
				AST_uptr constructor,
				AST_uptr block,
				LexScope lexScope,
				TypeMemberScope typeMemberScope)
				: typeName(MOVE(typeName)),
				constructor(MOVE(constructor)),
				block(MOVE(block)),
				lexScope(lexScope),
				typeMemberScope(typeMemberScope) {}
			ACCEPT_VISITOR_DECL;
		};
		class BinaryExpression : public Expression {
		public:
			AST_uptr left;
			AST_uptr right;
			token_t opToken;
			BinaryExpression() {}
			BinaryExpression(AST_uptr left,
				AST_uptr right,
				const token_t opToken)
				: left(MOVE(left)),
				right(MOVE(right)),
				opToken(opToken) {}
			ACCEPT_VISITOR_DECL;
		};
		class UnaryExpression : public Expression {
		public:
			AST_uptr exp;
			token_t opToken;
			UnaryExpression() {}
			UnaryExpression(AST_uptr exp,
				const token_t opToken)
				: exp(MOVE(exp)),
				opToken(opToken) {}
			ACCEPT_VISITOR_DECL;
		};
		class FunctionBody : public Statement {
		public:
			AST_uptr paramList;
			AST_uptr block;
			size_t line;
			FunctionBody() {}
			FunctionBody(AST_uptr paramList,
				AST_uptr block,
				size_t line)
				: paramList(MOVE(paramList)),
				block(MOVE(block)),
				line(line) {}
			ACCEPT_VISITOR_DECL;
		};
		class ParamList : public Expression {
		public:
			AST_uptr name;
			bool vararg;
			size_t argCount;
			ParamList(AST_uptr name,
				bool vararg)
				: name(MOVE(name)),
				vararg(vararg),
				argCount(0) {}
			ACCEPT_VISITOR_DECL;
		};
		class NameList : public Expression {
		public:
			AST_uptr name;
			NameList() {}
			ACCEPT_VISITOR_DECL;
		};
		class NormalFuncCall : public Statement {
		public:
			AST_uptr caller;
			std::vector<Expression> args;
			size_t line;
			NormalFuncCall(AST_uptr caller,
				std::vector<Expression> args,
				size_t line)
				: caller(MOVE(caller)),
				args(MOVE(args)), line(line) {}
			ACCEPT_VISITOR_DECL;
		};
		class MemberFuncCall : public Statement {
		public:
			AST_uptr caller;
			token_t member;
			std::vector<Expression> args;
			size_t line;
			MemberFuncCall(AST_uptr caller,
				const token_t member,
				std::vector<Expression> args,
				size_t line)
				: caller(MOVE(caller)),
				member(member),
				args(MOVE(args)),
				line(line) {}
			ACCEPT_VISITOR_DECL;
		};
		class FuncCallArgs : public Expression {
		public:
			std::vector<Expression> args;
			enum ArgType { ExpList, Array, Other } type;
			size_t argValueCount_;
			FuncCallArgs(std::vector<Expression> args,
				ArgType type)
				: args(MOVE(args)),
				type(type),
				argValueCount_(0) {}
			ACCEPT_VISITOR_DECL;
		};
		class ExterneStatement : public Statement {
		public:
			token_t asmCode;
			ExterneStatement(const token_t asmCode)
				: asmCode(asmCode) {}
			ACCEPT_VISITOR_DECL;
		};
		class TryCatchExpression : public Statement {
		public:
			AST_uptr exceptionName;
			AST_uptr exceptionType;
			AST_uptr tryBlock;
			AST_uptr catchBlock;
			TryCatchExpression(AST_uptr exceptionName,
				AST_uptr exceptionType,
				AST_uptr tryBlock,
				AST_uptr catchBlock)
				: exceptionName(MOVE(exceptionName)),
				exceptionType(MOVE(exceptionType)),
				tryBlock(MOVE(tryBlock)),
				catchBlock(MOVE(catchBlock)) {}
			ACCEPT_VISITOR_DECL;
		};
		class VariableEdit : public Statement {
			std::string variableIdentifier;
			Expression exprValue;
			VariableEdit(std::string variableIdentifier,
				Expression exprValue)
				: variableIdentifier(MOVE(variableIdentifier)),
				exprValue(exprValue) {}
			ACCEPT_VISITOR_DECL;
		};
	}
}
