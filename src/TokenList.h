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
		LET,							// let
		TYPE,							// type
		TYPED,							// typed
		IMPLEMENT,						// implement
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
		IF,								// if
		ELIF,							// elif
		ELSE,							// else
		MATCH,							// match
		CASE,							// case
		DEFAULT,						// default
		EXTERNE,						// externe
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
		IT,								// it
		PUBLIC,							// public
		PRIVATE,						// private
		EXTERNAL,						// external
		UPON,							// upon
		/* Reserved Symbols */
		BEF,							/// used as enum delimiter for this compiler
		LONG_RIGHT_ARROW,				// -->
		DOUBLEPOINT,					// :
		SUCHAS,							// |
		AND,							// &&
		OR,								// ||
		PLUS,							// +
		MINUS,							// -
		TIME,							// *
		DIVIDE,							// /
		MODULO,							// %
		POWER,							// **
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

		TERNARY,						// ?

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
		SIZEOF,							// sizeof
		/* Dev inserts */
		NUMBER,							// 0, 1, 2, 3, 4, 5, 5, 7, 8, 9
		CHAR,							// '...'
		STRING,							// "..."
		BOOLEAN,						// true / false
		TYPENAME,						// typename
		TYPESIZE,						// typesize
		IDENTIFIER,
		UNKNOWN,
		NOTHING,
		/* AST node additional constants */

	};
	
	static const std::map<std::string, TokenList> KeywordList =
	{ {"var", VAR}, { "func" , FUNC}, { "let" , LET}, { "type" , TYPE}, { "typed" , TYPED},
	{ "enum" , ENUM}, { "namespace" , NAMESPACE}, { "while" , WHILE}, { "for" , FOR},
	{ "to" , TO}, { "structure" , STRUCTURE}, { "return" , RETURN}, { "implement", IMPLEMENT},
	{ "in" , _IN },{ "step" , STEP }, { "continue" , CONTINUE}, { "set" , SET}, { "if" , IF},
	{ "elif" , ELIF}, { "else" , ELSE}, { "match" , MATCH}, { "case" , CASE}, { "upon" , UPON},
	{ "externe" , EXTERNE}, { "each" , EACH}, { "true" , _TRUE}, { "false" , _FALSE},
	{ "is" , IS}, { "isnt" , ISNT}, { "it" , IT}, { "public" , PUBLIC}, { "private" , PRIVATE},
	{ "external" , EXTERNAL}, { "try" , TRY}, { "catch" , CATCH}, { "throw" , THROW},{ "new" , NEW},
	{ "delete" , _DELETE}, { "free" , FREE}, { "import" , IMPORT}, { "define" , DEFINE},
	{"sizeof", SIZEOF}, { "typename" , TYPENAME}, { "typesize", TYPESIZE } };
	static const std::map<std::string, TokenList> SymbolList =
	{ { "-->" , LONG_RIGHT_ARROW}, {":", DOUBLEPOINT}, { "|" , SUCHAS},
	{ "&&" , AND}, { "||" , OR}, { "+" , PLUS}, { "-" , LESS}, { "*", TIME},
	{ "/" , DIVIDE}, { "%" , MODULO}, { "**" , POWER}, { "++" , INC}, { "--" , DEC},
	{ "+=" , PLUS_EQUAL}, { "-=" , LESS_EQUAL}, { "/=" , DIVIDE_EQUAL}, { "*=" , TIME_EQUAL},
	{ "%=" , MODULO_EQUAL}, { "^" , CONCAT}, { "^=" , CONCAT_EQUAL}, { "^+" , CONCAT_PLUS},
	{ "^-" , CONCAT_LESS}, { "^/" , CONCAT_DIVIDE}, { "^*" , CONCAT_TIME},
	{ "^%" , CONCAT_MODULO}, { "==" , ISEQUAL}, { "=" , EQUAL}, { "<" , LESS},
	{ ">" , GREATER}, { "<=" , LESSEQ}, { ">=" , GREATEREQ}, { "!=" , ISNOTEQUAL},
	{ "!" , OPPOSITE}, { "[" , LEFT_HOOK}, { "]" , RIGHT_HOOK}, { "(" , LEFT_PARENTHESIS},
	{ ")" , RIGHT_PARENTHESIS}, { "{" , LEFT_BRACE}, { "}" , RIGHT_BRACE}, { "." , DOT},
	{ "@" , NAMESPACE_CALLING}, { "," , COMMA}, { ";" , ENDLINE}, { "&" , POINTER_ADRESS},
	{ "~" , POINTER_VALUE}, { "?", TERNARY }
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
		if (token == TokenList::BOOLEAN) return "boolean";
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
			!System::Text::IsNumeric(System::Text::CharToString(token[0])) &&
			!System::Text::ContainsSpecialChar(token.substr(1, token.length()), "0123456789")
		);
	}
	inline bool IsCorrectToken(std::string token) {
		return (IsKeyword(token) || IsSymbol(token));
	}
	inline TokenList ToToken(std::string token) {
		if (token.empty()) return TokenList::NOTHING;
		if (token == "true" || token == "false") return TokenList::BOOLEAN;
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
