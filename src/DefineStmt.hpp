#pragma once
#include "System.hpp"
#include "Expression.hpp"
#include "Errors.hpp"


/*
	+------------------------------------------------------------------------------------+
	| This file manages preprocessor definitions.                                        |
	| None can have the same identifier, and they are accessible throughout the program, |
	| but not accessible if called before being defined.                                 |
	| All directives are stored in the following form:                                   |
	| <identifier, value>                                                                |
	+------------------------------------------------------------------------------------+
*/

namespace Parser {
	class DefineStatement {
	private:
		Exception exception;
		std::map<std::string /*name*/, Expr /*value*/> list;
	public:
		// Check if preprocessor definition is good
		bool IsGood(Expr expr);
		// Check if a preprocessor definition already exist
		bool IsDefined(std::string DefName);
		void append(Expr expr);
		void AppendRange(DefineStatement Define);
		Expr get(std::string name);
	};
}
