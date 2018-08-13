#pragma once
#include "System.hpp"
#include "Lexer.hpp"
#include "Expression.hpp"
#include "TokenList.h"
#include "Errors.hpp"

namespace tast {
	class Ast {
	private:
		struct Node;
		struct rlvalue {
			struct Node *node;
			bool IsEmpty;
			token_t value;
		};
		typedef rlvalue Left;
		typedef rlvalue Right;
		struct Node {
			token_t value;		// value of node ( Math symbols + condition symboles + assignation symboles )
			Left left;		    // left child of node  ( can be empty )
			Right right;        // right child of node ( can be empty )
		};

		typedef std::vector<Node> AST;

		enum AstStatementTypes {
			/*  | Types |           				 | tokens |                           | example |    */

			/* Declarations */
			FunctionDecl,						// func									func foo(a, b = x + y)[] : T;
			RuleDecl,							// instance ret / rule					instance ret foo [] : T = v;
			StructureDecl,						// structure							structure foo;
			ObjectDecl,							// instance structure / object			instance structure <t : T> foo : bar;
			VariableDecl,						// var									var foo[] : T = a ^+ b;
			RetDecl,							// ret									ret foo[] : T = a ^+ b;
			ReturnStmt,							// return								return a + b;
			/* Expressions */
			ForStmt,							// for									for (i = 0 to 10 step 2);
			WhileStmt,							// while								while (a != b);
			Each,								// each									each (x in y);
			IfStmt,								// if									if (a + b == c);
			ElifStmt,							// elif									elif (a + b == c);
			MatchStmt,							// match								match (a --> b in c);
			CaseStmt,							// case									case (a);
			ThrowStmt,							// throw								throw a;
			CatchStmt,							// catch								catch (a : T)
			/* without node */
			DeleteStmt,							// delete								delete foo;
			NewStmt,							// new									new foo;
			FreeStmt,							// free									free foo;
			SetStmt,							// set									set foo;
			ElseStmt,							// else									else;
			DefaultStmt,						// default								default;
			NamespaceDecl,						// namespace							namespace foo;
			EnumDecl,							// enum									enum foo;
			ContinueStmt,						// continue								continue;
			TryStmt,							// try									try;
			ExternStmt,							// extern								extern "";
			InstanceDecl,						// instance								instance foo;
			FunctionCalling,					// ...()								foo(a, b, c);
			IdentifierEdit,						// ...									a += b;
			Implement,							// implement							implement foo;

			UNKNOWN // ?
		};
		enum NodeTypes {
			START = TokenList::BEF, // enum start indicator

			LONG_RIGHT_ARROW =					// -->									a --> b
			TokenList::LONG_RIGHT_ARROW,
			INSTANCE_ENUM_STRUCTURE_OBJCALL,	// :									a : T
			WITH,								// |									a | b
			AND,								// &&									a && b
			OR,									// ||									a || b
			PLUS,								// +									a + b
			MINUS,								// -									a - b
			TIME,								// *									a * b
			DIVIDE,								// /									a / b
			MODULO,								// %									a % b
			INC,								// ++									a++
			DEC,								// --									a--
			ISEQUAL,							// ==									a == b
			EQUAL,								// =									a = b
			LESS,								// <									a < b
			GREATER,							// >									a > b
			LESSEQ,								// <=									a <= b
			GREATEREQ,							// >=									a >= b
			ISNOTEQUAL,							// !=									a != b
			OPPOSITE,							// !									!a

			TERNARY,							// ?									a . b ? a : b

			PLUS_EQUAL,							// +=									a += b
			LESS_EQUAL,							// -=									a -= b
			TIME_EQUAL,							// *=									a *= b
			DIVIDE_EQUAL,						// /=									a /= b
			MODULO_EQUAL,						// %=									a %= b

			CONCAT,								// ^									a ^ b
			CONCAT_EQUAL,						// ^=									a ^= b
			CONCAT_PLUS,						// ^+									a ^+ b
			CONCAT_LESS,						// ^-									a ^- b
			CONCAT_TIME,						// ^*									a ^* b
			CONCAT_DIVIDE,						// ^/									a ^/ b
			CONCAT_MODULO,						// ^%									a ^% b

			To =								// to									a to b
			TokenList::TO,
			Is = 								// is									a is b
			TokenList::IS,
			Isnt = 								// isnt									a isnt b
			TokenList::ISNT,
			In = 								// in									a in b
			TokenList::_IN,

			END = 68, // enum end indicator
			UnknownNode
		};

		AST ast;
		Exception exception;

		AstStatementTypes DetermineStatement(TokenList::TokenList TokenType, Expr expr);
		NodeTypes DetermineFirstNode(Expr expr);
		bool IsNode(token_t token);
		bool ContainNodes(Expr expr);
		void add(AstStatementTypes StatementType, NodeTypes NodeType);

	public:
		void append(Expr expr);
	};
}
