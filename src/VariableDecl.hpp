#pragma once
#include "System.hpp"
#include "Object.hpp"
#include "Expression.hpp"

namespace Parser {
	struct VariableDecl_t {
		std::string Identifier;
		type_t Type;
		Expr Value;
		bool IsArray;
		bool IsPrivate;
		bool IsPointer;  // ex: var MyRef = &MyVar; ~MyRef = 9;
		location_t Position;
	};
}
