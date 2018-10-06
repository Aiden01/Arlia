#pragma once
#include "System.hpp"
#include "Expression.hpp"
#include "Errors.hpp"
#include "StatementTypes.hpp"

#include "types.hpp"

namespace Parser {
	class Maker {
	private:
		/* defines the prefix of each instruction name in the block being processed according to
		   namespaces, functions, conditions, loops,... */
		std::string CurrentPrefixAbstrBlock;
		Exception exception;

		Types types;

		// --- statement / declaration makes

		void make_FuncDecl(Expr);
		void make_TypeDecl(Expr);
		void make_VarDecl(Expr);
		void make_LetDecl(Expr);
		void make_ReturnStmt(Expr);
		void make_ForStmt(Expr);
		void make_WhileStmt(Expr);
		void make_MatchStmt(Expr);
		void make_IfStmt(Expr);
		void make_ElifStmt(Expr);
		void make_ElseStmt(Expr);
		void make_ThrowStmt(Expr);
		void make_CatchStmt(Expr);
		void make_DeleteStmt(Expr);
		void make_FreeStmt(Expr);
		void make_NameSpaceDecl(Expr);
		void make_ContinueStmt(Expr);
		void make_TryStmt(Expr);
		void make_ExterneStmt(Expr);
		void make_FunctionCall(Expr);
		void make_VarEdit(Expr);

	public:
		void make(StatementTypes::StatementTypes StatementType, Expr expr);
		void startBlock();
		void endBlock();
	};
}
