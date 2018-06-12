#pragma once
#include "System.hpp"

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
		TO,								// to
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
		ENDLINE,						// ;
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
		case keywords::TO:
			return "to";
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
		case keywords::ENDLINE:
			return ";";
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
	bool IsKeyword(std::string word) {
		if (word == "var") return true;
		else if (word == "func") return true;
		else if (word == "ret") return true;
		else if (word == "data") return true;
		else if (word == "enum") return true;
		else if (word == "namespace") return true;
		else if (word == "static") return true;
		else if (word == "while") return true;
		else if (word == "for") return true;
		else if (word == "to") return true;
		else if (word == "structure") return true;
		else if (word == "yield") return true;
		else if (word == "set") return true;
		else if (word == "await") return true;
		else if (word == "if") return true;
		else if (word == "elif") return true;
		else if (word == "else") return true;
		else if (word == "match") return true;
		else if (word == "extern") return true;
		else if (word == "each") return true;
		else if (word == "true") return true;
		else if (word == "false") return true;
		else if (word == "is") return true;
		else if (word == "it") return false;			// Causes a bug during tokenization
		else if (word == "on") return true;
		else if (word == "off") return true;
		else if (word == "upon") return true;
		else if (word == "new") return true;
		else if (word == "delete") return true;
		else if (word == "free") return true;
		else if (word == "import") return true;
		else if (word == "define") return true;
		else if (word == "goto") return true;
		else if (word == "proc") return true;
		else if (word == "typename") return false;		// Because it's some kind of data type,
		else if (word == "typesize") return false;		// and a type is not a keyword.
		else return false;
	}
	bool IsSymbol(std::string symbol) {
		if (symbol == "->") return true;
		if (symbol == "-->") return true;
		if (symbol == ":") return true;
		if (symbol == "|") return true;
		if (symbol == "&&") return true;
		if (symbol == "||") return true;
		if (symbol == "+") return true;
		if (symbol == "-") return true;
		if (symbol == "*") return true;
		if (symbol == "/") return true;
		if (symbol == "%") return true;
		if (symbol == "==") return true;
		if (symbol == "=") return true;
		if (symbol == "<") return true;
		if (symbol == ">") return true;
		if (symbol == "<=") return true;
		if (symbol == ">=") return true;
		if (symbol == "!=") return true;
		if (symbol == "!") return true;
		if (symbol == "[") return true;
		if (symbol == "]") return true;
		if (symbol == "{") return true;
		if (symbol == "}") return true;
		if (symbol == "(") return true;
		if (symbol == ")") return true;
		if (symbol == ".") return true;
		if (symbol == "@") return true;
		if (symbol == ",") return true;
		if (symbol == ";") return true;
		if (symbol == "&") return true;
		if (symbol == "~") return true;
		return false;
	}
	keywords ToKeyword(std::string keyword) {
		if (keyword == "var") return keywords::VAR;
		if (keyword == "func") return keywords::FUNC;
		if (keyword == "ret") return keywords::RET;
		if (keyword == "data") return keywords::DATA;
		if (keyword == "enum") return keywords::ENUM;
		if (keyword == "namespace") return keywords::NAMESPACE;
		if (keyword == "static") return keywords::STATIC;
		if (keyword == "while") return keywords::WHILE;
		if (keyword == "for") return keywords::FOR;
		if (keyword == "to") return keywords::TO;
		if (keyword == "structure") return keywords::STRUCTURE;
		if (keyword == "yield") return keywords::YIELD;
		if (keyword == "set") return keywords::SET;
		if (keyword == "await") return keywords::AWAIT;
		if (keyword == "if") return keywords::IF;
		if (keyword == "elif") return keywords::ELIF;
		if (keyword == "else") return keywords::ELSE;
		if (keyword == "match") return keywords::MATCH;
		if (keyword == "extern") return keywords::EXTERN;
		if (keyword == "each") return keywords::EACH;
		if (keyword == "true") return keywords::_TRUE;
		if (keyword == "false") return keywords::_FALSE;
		if (keyword == "is") return keywords::IS;
		if (keyword == "it") return keywords::IT;
		if (keyword == "on") return keywords::ON;
		if (keyword == "off") return keywords::OFF;
		if (keyword == "upon") return keywords::UPON;
		if (keyword == "->") return keywords::SIMPLE_RIGHT_ARROW;
		if (keyword == "-->") return keywords::LONG_RIGHT_ARROW;
		if (keyword == ":") return keywords::DATA_ENUM_STRUCTURE_OBJCALL;
		if (keyword == "|") return keywords::WITH;
		if (keyword == "&&") return keywords::AND;
		if (keyword == "||") return keywords::OR;
		if (keyword == "+") return keywords::PLUS;
		if (keyword == "-") return keywords::MINUS;
		if (keyword == "*") return keywords::TIME;
		if (keyword == "/") return keywords::DIVIDE;
		if (keyword == "%") return keywords::MODULO;
		if (keyword == "==") return keywords::ISEQUAL;
		if (keyword == "=") return keywords::EQUAL;
		if (keyword == "<") return keywords::LESS;
		if (keyword == ">") return keywords::GREATER;
		if (keyword == "<=") return keywords::LESSEQ;
		if (keyword == ">=") return keywords::GREATEREQ;
		if (keyword == "!=") return keywords::ISNOTEQUAL;
		if (keyword == "!") return keywords::INVERSE;
		if (keyword == "[") return keywords::LEFT_HOOK;
		if (keyword == "]") return keywords::RIGHT_HOOK;
		if (keyword == "{") return keywords::LEFT_BRACE;
		if (keyword == "}") return keywords::RIGHT_BRACE;
		if (keyword == "(") return keywords::LEFT_PARENTHESIS;
		if (keyword == ")") return keywords::RIGHT_PARENTHESIS;
		if (keyword == ".") return keywords::DOT;
		if (keyword == "@") return keywords::NAMESPACE_CALLING;
		if (keyword == ",") return keywords::COMMA;
		if (keyword == ";") return keywords::ENDLINE;
		if (keyword == "&") return keywords::POINTER_ADRESS;
		if (keyword == "~") return keywords::POINTER_VALUE;
		if (keyword == "new") return keywords::NEW;
		if (keyword == "delete") return keywords::_DELETE;
		if (keyword == "free") return keywords::FREE;
		if (keyword == "define") return keywords::DEFINE;
		if (keyword == "goto") return keywords::GOTO;
		if (keyword == "proc") return keywords::PROC;
		if (keyword == "typename") return keywords::TYPENAME;
		if (keyword == "typesize") return keywords::TYPENAME;
		// ---- identifier
		if (!IsKeyword(keyword) &&
			!isdigit(keyword[0]) &&
			!System::Text::ContainsSpecialChar(keyword.substr(1, keyword.length()), "_0123456789") &&
			!keyword.empty())
			return keywords::IDENTIFIER;
		// ----
		if (System::Text::IsNumeric(keyword)) return keywords::NUMBER;
		if (System::Text::IsChar(keyword)) return keywords::CHAR;
		if (System::Text::IsString(keyword)) return keywords::STRING;
		return keywords::UNKNOWN;
	}
}
