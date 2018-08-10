#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Location.hpp"
#include "Expression.hpp"

namespace Parser {
	
	struct FunctionDecl_t;

	enum AstStatementTypes {
		/* Declarations */
		FunctionDecl,
		RuleDecl,
		ObjectDecl,
		InstanceDecl,
		EnumDecl,
		NamespaceDecl,
		VariableDecl,
		RetDecl,
		/* Statements */
		ForStmt,
		WhileStmt,
		DoStmt,
		ReturnStmt,
		IfStmt,
		ElifStmt,
		ElseStmt,
		ExternStmt,
		MatchStmt,
		CaseStmt,
		DefaultStmt,
		ContinueStmt,
		ThrowStmt,
		TryStmt,
		CatchStmt,
		SetStmt,
		DeleteStmt,
		NewStmt,
		FreeStmt,
		PublicStmt,
		PrivateStmt,
		StaticStmt,
		UponStmt,
		/* Calling / using */
		FunctionCalling,

	};

	namespace AST {

		struct Node {

		};

		struct Instruction {
			AstStatementTypes StatementTypes;

		};

		class AST {
		private:

			std::vector<Instruction> AstProgramObj;

			/// Declarations

			void Add_Function_Decl(struct FunctionDecl_t, Expr);
			void Add_Object_Decl(struct ObjectDecl_t, Expr);
			void Add_Instance_Decl(struct InstanceDecl_t, Expr);
			void Add_Variable_Decl(struct VariableDecl_t, Expr);
			void Add_Rule_Decl(struct RuleDecl_t, Expr);
			void Add_Ret_Decl(struct RetDecl_t, Expr);
			void Add_Enum_Decl(struct EnumDecl_t, Expr);
			void Add_Namespace_Decl(struct NamespaceDecl_t, Expr);

			/// Statements

			void Add_For_Stmt(struct ForStmt, Expr);
			void Add_While_Stmt(struct WhileStmt, Expr);
			void Add_Do_Stmt(struct WhileStmt, Expr);
			void Add_Return_Stmt(struct Expression, Expr);
			void Add_If_Stmt(struct IfStmt, Expr);
			void Add_Elif_Stmt(struct ElifStmt, Expr);
			void Add_Else_Stmt(struct ElseStmt, Expr);
			void Add_Extern_Stmt(struct ExternStmt, Expr);
			void Add_Match_Stmt(struct MatchStmt, Expr);
			void Add_Case_Stmt(struct CaseStmt, Expr);
			void Add_Default_Stmt(struct DefaultStmt, Expr);
			void Add_Continue_Stmt(struct ContinueStmt, Expr);
			void Add_Throw_Stmt(struct ThrowStmt, Expr);
			void Add_Try_Stmt(struct TryStmt, Expr);
			void Add_Catch_Stmt(struct CatchStmt, Expr);
			void Add_Set_Stmt(struct SetStmt, Expr);
			void Add_Delete_Stmt(struct DeleteStmt, Expr);
			void Add_New_Stmt(struct NewStmt, Expr);
			void Add_Free_Stmt(struct FreeStmt, Expr);
			void Add_Public_Stmt(struct PublicStmt, Expr);
			void Add_Private_Stmt(struct PrivateStmt, Expr);
			void Add_Static_Stmt(struct StaticStmt, Expr);
			void Add_Upon_Stmt(struct UponStmt, Expr);

			/// Calling / using

			void Add_Function_Calling(struct FuncCalling);

		public:
			void AddInstruction(AstStatementTypes, Expr);
			void PrintAst();
		};

	}
}
