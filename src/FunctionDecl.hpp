#pragma once
#include "System.hpp"
#include "Location.hpp"
#include "Object.hpp"
#include "Errors.hpp"
#include "TokenList.h"
#include "Assembler.hpp"

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
	inline bool FunctionAlreadyExist(std::string identifier) {
		for (std::vector<FunctionDecl_t>::iterator it = ListOfFunctions.begin(); it != ListOfFunctions.end(); ++it)
			if (it->Identifier == identifier) return true;
		return false;
	}

	// func MyFunc(args) : type { }

	inline bool IsAcceptableFunction(FunctionDecl_t decl, std::vector<token_t> line) {
		Exception exception;
		bool ret = true;
		if (FunctionAlreadyExist(decl.Identifier)) {
			exception.ThrowError(Exception::E0010, line[1]);
			ret = false;
		}
		if (decl.Identifier == "vanaur") {
			exception.ThrowError(Exception::E0033, line[3]);
			ret = false;
		}
		if (!TypeAlreadyExist(decl.Type.Identifier)) {
			exception.ThrowError(Exception::E0072, line.back());
			ret = false;
		}
		for (Argument arg : decl.Arguments) {
			if (!TokenList::IsIdentifier(arg.Identifier)) {
				exception.ThrowError(exception.E0028, line[3]);
				ret = false;
			}
			if (!TypeAlreadyExist(arg.Type.Identifier)) {
				exception.ThrowError(Exception::E0072, line[3]);
				ret = false;
			}
			if (arg.Identifier == "vanaur") {
				exception.ThrowError(Exception::E0033, line[3]);
				ret = false;
			}
			
		}
		return ret;
	}

	inline void AddFunction(FunctionDecl_t decl, std::vector<token_t> line) {
		if (!IsAcceptableFunction) return;
		ListOfFunctions.push_back(decl);
	}

}
