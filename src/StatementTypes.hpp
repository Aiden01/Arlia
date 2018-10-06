#pragma once
#include "TokenList.h"

namespace StatementTypes {

	enum StatementTypes {
		/*  | Types |           				 | tokens |                           | example |    */

		/* Declarations */
		FunctionDecl,						// func									func foo(a, b = x + y)[] : T;
		TypeDecl,							// type									type foo;
		VariableDecl,						// var									var foo[] : T = a ^+ b;
		LetDecl,							// ret									let foo[] : T = a ^+ b;
		ReturnStmt,							// return								return a + b;
											/* Expressions */
		ForStmt,							// for									for (i = 0 to 10 step 2);
		WhileStmt,							// while								while (a != b);
		IfStmt,								// if									if (a + b == c);
		ElifStmt,							// elif									elif (a + b == c);
		MatchStmt,							// match								match (a --> b in c);
		ThrowStmt,							// throw								throw a;
		CatchStmt,							// catch								catch (a : T)

		/* Unique or no child - ast */

		DeleteStmt,							// delete								delete foo;
		FreeStmt,							// free									free foo;
		ElseStmt,							// else									else;
		NamespaceDecl,						// namespace							namespace foo;
		ContinueStmt,						// continue								continue;
		TryStmt,							// try									try;
		ExterneStmt,						// externe								externe "";
		FunctionCalling,					// ...()								foo(a, b, c);
		IdentifierEdit,						// ...									a += b;
		ImportStmt,							// import								import file;
		DefineStmt,							// define								define FOO;
		EnumSuite,							//										a, b, c, d, ..., k

		/* Object member type */

		PublicMember,						// public
		PrivateMember,						// private

		SingleToken,
		NoDefined,
		Nothing,

		UNKNOWN // ?
	};

}