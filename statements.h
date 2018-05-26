#pragma once
#include <iostream>
#include <string>

namespace keywords {

	enum keywords {
		// Statements
		VAR,
		FUNC,
		RET,
		DATA,
		ENUM,
		NAMESPACE,
		STATIC,
		WHILE,
		FOR,
		STRUCTURE,
		YIELD,
		SET,
		AWAIT,
		IF,
		ELIF,
		ELSE,
		MATCH,
		EXTERN,
	
		// CrossDeclarationKeyword 
		EACH,
		_TRUE,
		_FALSE,
		IS,
	
		// DataStructureReservedKeywords 
		IT,		// *this*
		ON,		// *public*
		OFF,	// *private*
		UPON,	// *shadow / operator*
	
		// ReservedSymbols 
		SIMPLE_RIGHT_ARROW,				// ->
		LONG_RIGHT_ARROW,				// -->
		DATA_ENUM_STRUCTURE_OBJCALL,	// :
		WITH,							// |
	
		// ElementaryDataType 
		CHAR,		// 'a' || 97
		INTEGER,	// 87, 121, 98908, ...
		FLOAT,		// 87.99, 7867.00001, ...
		BOOLEAN,		// true/false || 1/0
	
		// PooKeywords 
		NEW,
	
		// MemoryKeywords 
		_DELETE,
		FREE,
	
		// Preprocessor 
		IMPORT,
		DEFINE,
		GOTO,
		PROC
	};

	std::string ToString(keywords keyword) {
		switch (keyword)
		{
		case keywords::VAR:
			return "var";
		case keywords::FUNC:
			return "func";
		case keywords::RET:
			return "ret";
		case keywords::DATA:
			return "data";
		case keywords::ENUM:
			return "enum";
		case keywords::NAMESPACE:
			return "namespace";
		case keywords::STATIC:
			return "static";
		case keywords::WHILE:
			return "while";
		case keywords::FOR:
			return "for";
		case keywords::STRUCTURE:
			return "structure";
		case keywords::YIELD:
			return "yield";
		case keywords::SET:
			return "set";
		case keywords::AWAIT:
			return "await";
		case keywords::IF:
			return "if";
		case keywords::ELIF:
			return "elif";
		case keywords::ELSE:
			return "else";
		case keywords::MATCH:
			return "match";
		case keywords::EXTERN:
			return "extern";
		case keywords::EACH:
			return "each";
		case keywords::_TRUE:
			return "true";
		case keywords::_FALSE:
			return "false";
		case keywords::IS:
			return "is";
		case keywords::IT:
			return "it";
		case keywords::ON:
			return "on";
		case keywords::OFF:
			return "off";
		case keywords::UPON:
			return "upon";
		case keywords::SIMPLE_RIGHT_ARROW:
			return "->";
		case keywords::LONG_RIGHT_ARROW:
			return "-->";
		case keywords::DATA_ENUM_STRUCTURE_OBJCALL:
			return ":";
		case keywords::WITH:
			return "|";
		case keywords::CHAR:
			return "char";
		case keywords::INTEGER:
			return "integer";
		case keywords::FLOAT:
			return "float";
		case keywords::BOOLEAN:
			return "boolean";
		case keywords::NEW:
			return "new";
		case keywords::_DELETE:
			return "delete";
		case keywords::FREE:
			return "free";
		case keywords::IMPORT:
			return "import";
		case keywords::DEFINE:
			return "define";
		case keywords::GOTO:
			return "goto";
		case keywords::PROC:
			return "proc";
		}
	}

}