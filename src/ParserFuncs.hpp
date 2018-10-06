#pragma once
#include "Parser.hpp"

namespace Parser {
	/* functions */
	enum TypeOfFuncCall {
		// @namespace:object.function()  [6] -> 7
		// @namespace.function()		 [4] -> 5
		// object.function()			 [3] -> 4
		// function()					 [1] -> 3
		NamespaceObject_Function, // @namespace:object.function()
		Namespace_Function,		  // @namespace:function()
		Object_Function,		  // object.function()
		_Function,				  // function()
		UNKNOWN
	};
	bool isNamespaceObject_Function(Expr);
	bool isNamespace_Function(Expr);
	bool isObject_Function(Expr);
	bool is_Function(Expr);
	TypeOfFuncCall TypeOfFunctionCall(Expr);
}

