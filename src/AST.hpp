#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Location.hpp"

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

			void Add_Function_Decl(struct FunctionDecl_t, std::vector<token_t> line);
			void Add_Object_Decl(struct ObjectDecl_t, std::vector<token_t> line);
			void Add_Instance_Decl(struct InstanceDecl_t, std::vector<token_t> line);
			void Add_Variable_Decl(struct VariableDecl_t, std::vector<token_t> line);
			void Add_Rule_Decl(struct RuleDecl_t, std::vector<token_t> line);
			void Add_Ret_Decl(struct RetDecl_t, std::vector<token_t> line);
			void Add_Enum_Decl(struct EnumDecl_t, std::vector<token_t> line);
			void Add_Namespace_Decl(struct NamespaceDecl_t, std::vector<token_t> line);

			/// Statements

			void Add_For_Stmt(struct ForStmt, std::vector<token_t> line);
			void Add_While_Stmt(struct WhileStmt, std::vector<token_t> line);
			void Add_Do_Stmt(struct WhileStmt, std::vector<token_t> line);
			void Add_Return_Stmt(struct Expr, std::vector<token_t> line);
			void Add_If_Stmt(struct IfStmt, std::vector<token_t> line);
			void Add_Elif_Stmt(struct ElifStmt, std::vector<token_t> line);
			void Add_Else_Stmt(struct ElseStmt, std::vector<token_t> line);
			void Add_Extern_Stmt(struct ExternStmt, std::vector<token_t> line);
			void Add_Match_Stmt(struct MatchStmt, std::vector<token_t> line);
			void Add_Case_Stmt(struct CaseStmt, std::vector<token_t> line);
			void Add_Default_Stmt(struct DefaultStmt, std::vector<token_t> line);
			void Add_Continue_Stmt(struct ContinueStmt, std::vector<token_t> line);
			void Add_Throw_Stmt(struct ThrowStmt, std::vector<token_t> line);
			void Add_Try_Stmt(struct TryStmt, std::vector<token_t> line);
			void Add_Catch_Stmt(struct CatchStmt, std::vector<token_t> line);
			void Add_Set_Stmt(struct SetStmt, std::vector<token_t> line);
			void Add_Delete_Stmt(struct DeleteStmt, std::vector<token_t> line);
			void Add_New_Stmt(struct NewStmt, std::vector<token_t> line);
			void Add_Free_Stmt(struct FreeStmt, std::vector<token_t> line);
			void Add_Public_Stmt(struct PublicStmt, std::vector<token_t> line);
			void Add_Private_Stmt(struct PrivateStmt, std::vector<token_t> line);
			void Add_Static_Stmt(struct StaticStmt, std::vector<token_t> line);
			void Add_Upon_Stmt(struct UponStmt, std::vector<token_t> line);

			/// Calling / using

			void Add_Function_Calling(struct FuncCalling);

		public:
			void AddInstruction(AstStatementTypes, std::vector<token_t>);
			void PrintAst();
		};

	}
}
