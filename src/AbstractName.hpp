#pragma once
#include "System.hpp"

class AbstractName {
public:
	typedef enum Blocks {
		NAMESPACE,	// namespace {}
		STRUCTURE,	// structure {}
		ENUM,		// enum {}
		FUNCTION,	// function () {}
		WHILE_LOOP,	// while () {}
		FOR_LOOP,	// for () {}
		CONDITION,	// if () {}
		BRACE		// {}
	};
	const std::string BlockName_namespace = "namespace";
	const std::string BlockName_structure = "structure";
	const std::string BlockName_enum = "enum";
	const std::string BlockName_func = "func";
	const std::string BlockName_while = "while";
	const std::string BlockName_for = "for";
	const std::string BlockName_if = "if";
	const std::string BlockName_brace = "{";
	Blocks BlockName(const std::string BlockName) {
		if (BlockName == "namespace") return Blocks::NAMESPACE;
		if (BlockName == "structure") return Blocks::STRUCTURE;
		if (BlockName == "enum") return Blocks::ENUM;
		if (BlockName == "func") return Blocks::FUNCTION;
		if (BlockName == "while") return Blocks::WHILE_LOOP;
		if (BlockName == "for") return Blocks::FOR_LOOP;
		if (BlockName == "if") return Blocks::CONDITION;
		if (BlockName == "{") return Blocks::BRACE;
	}
private:
	std::string Abstract_BlocksEnumToString(Blocks block, std::string name) {
		switch (block) {
		case AbstractName::NAMESPACE: return "__NMSPC_" + name + "_";
		case AbstractName::STRUCTURE: return "__STRCTR_" + name + "_";
		case AbstractName::ENUM: return "__NM_" + name + "_";
		case AbstractName::FUNCTION: return "__FNCTN_" + name + "_";
		case AbstractName::WHILE_LOOP: return "__WHLLP_" + name + "_";
		case AbstractName::FOR_LOOP: return "__FRLP_" + name + "_";
		case AbstractName::CONDITION: return "__CNDTN_" + name + "_";
		case AbstractName::BRACE: return "__BRC_" + name + "_";
		}
	}
public:
	std::string GetAbstractName(std::multimap<Blocks, std::string> blocks, std::string name) {
		std::string ret;
		for (std::map<Blocks, std::string>::iterator it = blocks.begin(); it != blocks.end(); ++it)
			ret += Abstract_BlocksEnumToString(it->first, it->second);
		return ret + name;
	}
};
 
