#pragma once
#include "System.hpp"
#include "Object.hpp"
#include "Expression.hpp"
#include "Errors.hpp"
#include "Parser.hpp"

namespace Parser {
	struct VariableDecl_t {
		std::string Identifier;
		std::string Adress; // ex: esp-4
		bool HasType;
		type_t Type;
		bool HasValue;
		Expr Value;
		bool IsArray;
		bool IsPrivate;
		bool IsPointer;  // ex: var MyRef = &MyVar; ~MyRef = 9;
		location_t Position;
	};
	typedef VariableDecl_t variable_t;

	static std::vector<variable_t> ListOfVariables;

	inline bool VariableAlreadyExist(std::string identifier) {
		for (std::vector<VariableDecl_t>::iterator it = ListOfVariables.begin(); it != ListOfVariables.end(); ++it)
			if (it->Identifier == identifier) return true;
		return false;
	}

	inline bool IsAcceptableVariable(VariableDecl_t decl, Expr line) {
		Exception exception;
		bool ret = true;
		if (VariableAlreadyExist(decl.Identifier)) {
			exception.ThrowError(Exception::E0009, line[1]);
			ret = false;
		}
		if (decl.Identifier == "vanaur") {
			exception.ThrowError(Exception::E0033, line[3]);
			ret = false;
		}
		if (decl.HasType == true) if (!TypeAlreadyExist(decl.Type.Identifier)) {
			exception.ThrowError(Exception::E0072, line[4]);
			ret = false;
		}

		// Check value !!

		return ret;
	}

	inline void AddVariable(VariableDecl_t decl, Expr line) {
		if (!IsAcceptableVariable) return;
		ListOfVariables.push_back(decl);
	}
	inline variable_t GetVariable(std::string identifier) {
		for (std::vector<VariableDecl_t>::iterator it = ListOfVariables.begin(); it != ListOfVariables.end(); ++it)
			if (it->Identifier == identifier) return ListOfVariables[it - ListOfVariables.begin()];
		variable_t ret;
		ret.Adress = identifier;
		return ret;
	}

}
