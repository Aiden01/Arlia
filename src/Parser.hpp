#pragma once
#include "System.hpp"
#include "Lexer.hpp"
#include "Assembler.hpp"
#include "Errors.hpp"
#include "Expression.hpp"
#include "DefineStmt.hpp"

namespace Parser {
	class parser {
	private:
		enum StatementTypes {
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
			ProcStmt,							// proc									proc a, "id", b;

			/* Unique or no child - ast */

			DeleteStmt,							// delete								delete foo;
			FreeStmt,							// free									free foo;
			SetStmt,							// set									set foo;
			ElseStmt,							// else									else;
			DefaultStmt,						// default								default;
			NamespaceDecl,						// namespace							namespace foo;
			EnumDecl,							// enum									enum foo;
			SingleToken,						// MyEnumValue							enum foo { MyEnumValue }
			ContinueStmt,						// continue								continue;
			TryStmt,							// try									try;
			ExterneStmt,						// externe								externe "";
			InstanceDecl,						// instance								instance foo;
			FunctionCalling,					// ...()								foo(a, b, c);
			IdentifierEdit,						// ...									a += b;
			ImplementStmt,						// implement							implement foo;
			ImportStmt,							// import								import file;
			DefineStmt,							// define								define FOO;
			GotoStmt,							// goto									goto label
			EnumSuite,							//										a, b, c, d, ..., k

			/* Object member type */

			PublicMember,						// public
			PrivateMember,						// private
			StaticMember,						// static
			UponMember,							// upon

			NoDefined,
			Nothing,

			UNKNOWN // ?
		};
		enum NodeTypes {
			START = TokenList::BEF,				// enum start indicator

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

			END = 68,						    // enum end indicator
			UnknownNode							// = value
		};
		StatementTypes DetermineStatement(TokenList::TokenList TokenType, Expr expr, Lexer &lexer);
		enum TypeOfEndExpr {
			EndLine,			// ';'
			RightParenthesis,	// ')'
			LeftBrace,			// '{'
			RightBrace			// '}'
		};

		Exception exception;

		// Checks if the expression that follows the statement ends with a semicolon or a right parenthesis or left brace.
		TypeOfEndExpr StatementEndExpr(StatementTypes StatementType);
		// Modify the expression sent in relation to the type of end of line as well as the type of exercise (local or general)
		void SetExprViaEndLineTypeAndLexer(Expr &expr, TypeOfEndExpr &TypeOfEndExpr, Lexer &lexer);
		void import(bool, Expr&, Lexer&, Lexer&, token_t , std::vector<std::string>&);

		void AppendToAst(StatementTypes StatementType, TypeOfEndExpr EnfOfExprType, Expr expr);
		void AppendToAst_StartingBlock(); // '{'
		void AppendToAst_EndingBlock();   // '}'

		int prestmt_NoDefined(token_t &token, Expr &expr, TokenList::TokenList TokenType, StatementTypes &StatementType, Lexer &lexer);

		void SetStatement(StatementTypes, Lexer &, Lexer &, std::vector<std::string> &, token_t , Expr &, bool);

		DefineStatement Define;
		void UpdateLexerDefine(Lexer &lexer, Lexer &LocalLexer);

	public:
		void parse(Lexer&, std::vector<std::string>&, std::string = "");
	};
}
