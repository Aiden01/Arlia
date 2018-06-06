#pragma once
#include <iostream>
#include <string>

namespace keywords {

	enum keywords {
		/* Statements */
		VAR,							// var
		FUNC,							// func
		RET,							// ret
		DATA,							// data
		ENUM,							// enum
		NAMESPACE,						// namespace
		STATIC,							// static
		WHILE,							// while
		FOR,							// for
		STRUCTURE,						// structure
		YIELD,							// yield
		SET,							// set
		AWAIT,							// await
		IF,								// if
		ELIF,							// elif
		ELSE,							// else
		MATCH,							// match
		EXTERN,							// extern
		/* Cross Declaration Keyword */ 
		EACH,							// each
		_TRUE,							// true
		_FALSE,							// false
		IS,								// is
		/* Data Structure Reserved Keywords */ 
		IT,								// it		// *this*
		ON,								// on		// *public*
		OFF,							// off		// *private*
		UPON,							// upon		// *shadow / operator*
		/* Reserved Symbols */ 
		SIMPLE_RIGHT_ARROW,				// ->
		LONG_RIGHT_ARROW,				// -->
		DATA_ENUM_STRUCTURE_OBJCALL,	// :
		WITH,							// |
		AND,							// &&
		OR,								// ||
		PLUS,							// +
		MINUS,							// -
		TIME,							// *
		DIVIDE,							// /
		MODULO,							// %
		ISEQUAL,						// ==
		EQUAL,							// =
		LESS,							// <
		GREATER,						// >
		LESSEQ,							// <=
		GREATEREQ,						// >=
		ISNOTEQUAL,						// !=
		INVERSE,						// !
		LEFT_HOOK,						// [
		RIGHT_HOOK,						// ]
		LEFT_BRACE,						// {
		RIGHT_BRACE,					// }
		LEFT_PARENTHESIS,				// (
		RIGHT_PARENTHESIS,				// )
		DOT,							// .
		NAMESPACE_CALLING,				// @
		COMMA,							// ,
		POINTER_ADRESS,					// &
		POINTER_VALUE,					// ~
		/* Poo - memory Keywords */
		NEW,							// new
		_DELETE,						// delete
		FREE,							// free
		/* Preprocessor */ 
		IMPORT,							// import
		DEFINE,							// define
		GOTO,							// goto
		PROC,							// proc
		/* Dev inserts */
		NUMBER,							// 0, 1, 2, 3, 4, 5, 5, 7, 8, 9
		CHAR,							// '...'
		STRING,							// "..."
		META,							// < ... >
		TYPENAME,						// typename
		TYPESIZE,						// typesize
		IDENTIFIER,						// a name, not a keyword / symbol / number
		UNKNOWN
	};
	std::string ToString(keywords keyword) {
		switch (keyword) {
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
		case keywords::AND:
			return "&&";
		case keywords::OR:
			return "||";
		case keywords::PLUS:
			return "+";
		case keywords::MINUS:
			return "-";
		case keywords::TIME:
			return "*";
		case keywords::DIVIDE:
			return "/";
		case keywords::MODULO:
			return "+";
		case keywords::ISEQUAL:
			return "==";
		case keywords::EQUAL:
			return "=";
		case keywords::LESS:
			return "<";
		case keywords::GREATER:
			return ">";
		case keywords::LESSEQ:
			return "<=";
		case keywords::GREATEREQ:
			return ">=";
		case keywords::ISNOTEQUAL:
			return "!=";
		case keywords::INVERSE:
			return "!";
		case keywords::LEFT_HOOK:
			return "[";
		case keywords::RIGHT_HOOK:
			return "]";
		case keywords::LEFT_BRACE:
			return "{";
		case keywords::RIGHT_BRACE:
			return "}";
		case keywords::LEFT_PARENTHESIS:
			return "(";
		case keywords::RIGHT_PARENTHESIS:
			return ")";
		case keywords::DOT:
			return ".";
		case keywords::NAMESPACE_CALLING:
			return "@";
		case keywords::COMMA:
			return ",";
		case keywords::POINTER_ADRESS:
			return "&";
		case keywords::POINTER_VALUE:
			return "~";
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
			/* values: */
		case keywords::NUMBER:
			return "number";
		case keywords::CHAR:
			return "char";
		case keywords::STRING:
			return "string";
		case keywords::META:
			return "meta";
		case keywords::TYPENAME:
			return "typename";
		case keywords::TYPESIZE:
			return "typesize";
		case keywords::IDENTIFIER:
			return "identifier";
		case keywords::UNKNOWN:
			return "unknown";
		}
	}

}
