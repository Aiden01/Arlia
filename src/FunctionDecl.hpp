#pragma once
#include "System.hpp"
#include "Location.hpp"
#include "Object.hpp"
#include "Errors.hpp"

namespace Parser {

	struct Argument {
		std::string Identifier;
		type_t Type;
		Expr Value;
		bool IsPointer;  // ex: var MyRef = &MyVar; ~MyRef = 9;
	};
	struct FunctionDecl_t {
		std::string Identifier;
		type_t Type;
		std::vector<Argument> Arguments;
		bool IsArray;
		bool IsPrivate;
		location_t Position;
	};

	static std::vector<FunctionDecl_t> ListOfFunctions;
	static Exception exception;
	inline bool FunctionAlreadyExist(std::string identifier) {
		for (std::vector<FunctionDecl_t>::iterator it = ListOfFunctions.begin(); it != ListOfFunctions.end(); ++it)
			if (it->Identifier == identifier) return true;
		return false;
	}

	inline void AddFunction(FunctionDecl_t decl, token_t identifier_tok) {
		if (FunctionAlreadyExist(decl.Identifier)) {
			exception.ThrowError(Exception::E0010, identifier_tok);
			return;
		}
		// Vérifier si tout est ok dans 'decl'
		ListOfFunctions.push_back(decl);
	}

}
