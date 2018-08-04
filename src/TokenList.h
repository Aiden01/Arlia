#pragma once
#include "System.hpp"
#include "Location.hpp"

/*
	+ -------------------------------------------------------------------------------------------------- - +
	| This file contains the entire list of language tokens as well as functions and utility constants. |
	+-------------------------------------------------------------------------------------------------- - +
*/


namespace TokenList {

	enum TokenList {
		/* Statements */
		VAR,							// var
		FUNC,							// func
		RET,							// ret
		INSTANCE,							// data
		ENUM,							// enum
		NAMESPACE,						// namespace
		WHILE,							// while
		FOR,							// for
		TO,								// to
		_IN,							// in
		STEP,							// step
		STRUCTURE,						// structure
		RETURN,							// return
		CONTINUE,						// continue
		SET,							// set
		AWAIT,							// await
		IF,								// if
		ELIF,							// elif
		ELSE,							// else
		MATCH,							// match
		CASE,							// case
		DEFAULT,						// default
		EXTERN,							// extern
		THROW,							// throw
		TRY,							// try
		CATCH,							// catch
		/* Cross Declaration Keyword */
		EACH,							// each
		_TRUE,							// true
		_FALSE,							// false
		IS,								// is
		ISNT,							// isnt
		/* Data Structure Reserved Keywords */
		IT,								// it		// *this*
		PUBLIC,							// public
		PRIVATE,						// private
		STATIC,							// static
		UPON,							// upon		// *shadow / operator*
		/* Reserved Symbols */
		LONG_RIGHT_ARROW,				// -->
		INSTANCE_ENUM_STRUCTURE_OBJCALL,	// :
		WITH,							// |
		AND,							// &&
		OR,								// ||
		PLUS,							// +
		MINUS,							// -
		TIME,							// *
		DIVIDE,							// /
		MODULO,							// %
		INC,							// ++
		DEC,							// --
		ISEQUAL,						// ==
		EQUAL,							// =
		LESS,							// <
		GREATER,						// >
		LESSEQ,							// <=
		GREATEREQ,						// >=
		ISNOTEQUAL,						// !=
		OPPOSITE,						// !

		PLUS_EQUAL,						// +=
		LESS_EQUAL,						// -=
		TIME_EQUAL,						// *=
		DIVIDE_EQUAL,					// /=
		MODULO_EQUAL,					// %=

		CONCAT,							// ^
		CONCAT_EQUAL,					// ^=
		CONCAT_PLUS,					// ^+
		CONCAT_LESS,					// ^-
		CONCAT_TIME,					// ^*
		CONCAT_DIVIDE,					// ^/
		CONCAT_MODULO,					// ^%

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
		IFDEF,							// ifdef
		ELIFDEF,						// elifdef
		GOTO,							// goto
		PROC,							// proc
		SIZEOF,							// sizeof
		/* Dev inserts */
		NUMBER,							// 0, 1, 2, 3, 4, 5, 5, 7, 8, 9
		CHAR,							// '...'
		STRING,							// "..."
		TYPENAME,						// typename
		TYPESIZE,						// typesize
		IDENTIFIER,						// a name, not a TokenList / symbol / number
		UNKNOWN,
		NOTHING,
		/* AST node additional constants */

	};
	
	static const std::map<std::string, TokenList> KeywordList =
	{ {"var", TokenList::VAR}, { "func" , TokenList::FUNC}, { "ret" , TokenList::RET}, { "instance" , TokenList::INSTANCE},
	{ "enum" , TokenList::ENUM}, { "namespace" , TokenList::NAMESPACE}, { "while" , TokenList::WHILE}, { "for" , TokenList::FOR},
	{ "to" , TokenList::TO}, { "structure" , TokenList::STRUCTURE}, { "return" , TokenList::RETURN},
	{ "in" , TokenList::_IN },{ "step" , TokenList::STEP }, { "continue" , TokenList::CONTINUE},
	{ "set" , TokenList::SET}, { "await" , TokenList::AWAIT}, { "if" , TokenList::IF}, { "elif" , TokenList::ELIF},
	{ "else" , TokenList::ELSE}, { "match" , TokenList::MATCH}, { "case" , TokenList::CASE}, { "upon" , TokenList::UPON},
	{ "extern" , TokenList::EXTERN}, { "each" , TokenList::EACH}, { "true" , TokenList::_TRUE}, { "false" , TokenList::_FALSE},
	{ "is" , TokenList::IS}, { "isnt" , TokenList::ISNT}, { "it" , TokenList::IT}, { "public" , TokenList::PUBLIC},
	{ "private" , TokenList::PRIVATE}, { "static" , TokenList::STATIC}, { "try" , TokenList::TRY}, { "catch" , TokenList::CATCH},
	{ "throw" , TokenList::THROW}, { "new" , TokenList::NEW}, { "delete" , TokenList::_DELETE}, { "free" , TokenList::FREE},
	{ "import" , TokenList::IMPORT}, { "define" , TokenList::DEFINE}, { "ifdef" , TokenList::IFDEF}, { "elifdef" , TokenList::ELIFDEF},
	{ "goto" , TokenList::GOTO}, { "proc" , TokenList::PROC}, {"sizeof", TokenList::SIZEOF}, { "typename" , TokenList::TYPENAME},
	{ "typesize", TokenList::TYPESIZE } };
	static const std::map<std::string, TokenList> SymbolList =
	{ { "-->" , TokenList::LONG_RIGHT_ARROW}, {":", TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL}, { "|" , TokenList::WITH},
	{ "&&" , TokenList::AND}, { "||" , TokenList::OR}, { "+" , TokenList::PLUS}, { "-" , TokenList::LESS}, { "*", TokenList::TIME},
	{ "/" , TokenList::DIVIDE}, { "%" , TokenList::MODULO}, { "++" , TokenList::INC}, { "--" , TokenList::DEC},
	{ "+=" , TokenList::PLUS_EQUAL}, { "-=" , TokenList::LESS_EQUAL}, { "/=" , TokenList::DIVIDE_EQUAL}, { "*=" , TokenList::TIME_EQUAL},
	{ "%=" , TokenList::MODULO_EQUAL}, { "^" , TokenList::CONCAT}, { "^=" , TokenList::CONCAT_EQUAL}, { "^+" , TokenList::CONCAT_PLUS},
	{ "^-" , TokenList::CONCAT_LESS}, { "^/" , TokenList::CONCAT_DIVIDE}, { "^*" , TokenList::CONCAT_TIME},
	{ "^%" , TokenList::CONCAT_MODULO}, { "==" , TokenList::ISEQUAL}, { "=" , TokenList::EQUAL}, { "<" , TokenList::LESS},
	{ ">" , TokenList::GREATER}, { "<=" , TokenList::LESSEQ}, { ">=" , TokenList::GREATEREQ}, { "!=" , TokenList::ISNOTEQUAL},
	{ "!" , TokenList::OPPOSITE}, { "[" , TokenList::LEFT_HOOK}, { "]" , TokenList::RIGHT_HOOK}, { "(" , TokenList::LEFT_PARENTHESIS},
	{ ")" , TokenList::RIGHT_PARENTHESIS}, { "{" , TokenList::LEFT_BRACE}, { "}" , TokenList::RIGHT_BRACE}, { "." , TokenList::DOT},
	{ "@" , TokenList::NAMESPACE_CALLING}, { "," , TokenList::COMMA}, { ";" , TokenList::ENDLINE}, { "&" , TokenList::POINTER_ADRESS},
	{ "~" , TokenList::POINTER_VALUE}
};
	const std::string NumberSuffix = "fcdrbs"; /*
												Number suffix:
													f : Float
													c : Char
													d : Decimal
													r : Real
													b : Boolean / byte
													s : String
											   */

	template<typename key_t, typename value_t>
	inline std::map<key_t, value_t> ConcatMap(std::map<key_t, value_t> map1, std::map<key_t, value_t> map2) {
		std::swap(map1, map2);
		map1.insert(map2.begin(), map2.end());
		return map1;
	}

	static std::map<std::string, TokenList> Tokens = ConcatMap<std::string, TokenList>(KeywordList, SymbolList);

	inline std::string ToString(TokenList token) {
		if (token == TokenList::IDENTIFIER) return "identifier";
		if (token == TokenList::NUMBER) return "number";
		if (token == TokenList::CHAR) return "character";
		if (token == TokenList::STRING) return "string";
		for (std::map<std::string, TokenList>::const_iterator it = Tokens.begin(); it != Tokens.end(); ++it)
			if (it->second == token) return it->first;
		return "unknown";
	}
	inline bool IsKeyword(std::string token) {
		std::map<std::string, TokenList>::const_iterator it = KeywordList.find(token);
		return (it != KeywordList.end());
	}
	inline bool IsSymbol(std::string token) {
		std::map<std::string, TokenList>::const_iterator it = SymbolList.find(token);
		return (it != SymbolList.end());
	}
	inline bool IsSymbol(char token) {
		std::map<std::string, TokenList>::const_iterator it = SymbolList.find(System::Text::CharToString(token));
		return (it != SymbolList.end());
	}
	inline bool IsIdentifier(std::string token) {
		return (
			!IsKeyword(token) &&
			!IsSymbol(token) &&
			!token.empty() &&
			!System::Text::IsNumeric(System::Text::CharToString(token[0]), NumberSuffix) &&
			!System::Text::ContainsSpecialChar(token.substr(1, token.length()), "0123456789")
		);
	}
	inline bool IsCorrectToken(std::string token) {
		return (IsKeyword(token) || IsSymbol(token));
	}
	inline TokenList ToToken(std::string token) {
		if (token.empty()) return TokenList::NOTHING;
		if (System::Text::IsNumeric(token, NumberSuffix)) return TokenList::NUMBER;
		if (System::Text::IsChar(token)) return TokenList::CHAR;
		if (System::Text::IsString(token)) return TokenList::STRING;
		for (std::map<std::string, TokenList>::const_iterator it = Tokens.begin(); it != Tokens.end(); ++it)
			if (token == it->first) return it->second;
		if (IsIdentifier(token)) return TokenList::IDENTIFIER;
		return TokenList::UNKNOWN;
	}
}

typedef TokenList::TokenList TYPE;

struct Token {
	TYPE type;
	std::string value;
	location_t position;
};

typedef Token token_t;
