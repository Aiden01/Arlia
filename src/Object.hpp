#pragma once
#include "System.hpp"
#include "template.hpp"
#include "Upon.hpp"
#include "LL_NameConventions.hpp"

namespace Parser {
	struct FunctionDecl_t;
	struct VariableDecl_t;

	struct Object {
		std::string Identifier;
		template_t Template;
		std::vector<Object> Inheritances;
		std::vector<FunctionDecl_t*> Methods;
		std::vector<VariableDecl_t*> Fields;
		std::vector<MetaUpon> Upons;
		size_t size;
	};

	typedef Object type_t;
	typedef Object ObjectDecl_t;

	static std::vector<type_t> ListOfTypes;

	inline bool TypeAlreadyExist(std::string identifier) {
		if (System::Text::IsNumeric(identifier.substr(2) /* cf: LL */, "", false)) return true;
		for (std::vector<type_t>::iterator it = ListOfTypes.begin(); it != ListOfTypes.end(); ++it)
			if (it->Identifier == identifier) return true;
		return false;
	}
	inline type_t GetType(std::string identifier) {
		for (std::vector<type_t>::iterator it = ListOfTypes.begin(); it != ListOfTypes.end(); ++it)
			if (it->Identifier == identifier) return ListOfTypes[it - ListOfTypes.begin()];
		return { identifier, {}, {}, {}, {}, {}, static_cast<size_t>(std::stoi(ConvertToHL(identifier))) };
	}

}
