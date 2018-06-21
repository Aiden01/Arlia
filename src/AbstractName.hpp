#pragma once
#include "System.hpp"

/*
	With nodes of AST
*/

namespace AbstractName {
	std::string _namespace(std::string NamespaceName) {
		return "_NMSPC" + NamespaceName;
	}
	std::string _structure(std::string StructureName) {
		return "_STRCTR" + StructureName;
	}
	std::string _function(std::string FunctionName) {
		return "_FNC" + FunctionName;
	}
}
