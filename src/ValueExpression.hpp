#pragma once
#include "System.hpp"
#include "TokenList.h"

enum TypesOfValue {
	String,			// "..."
	Char,			// '...'
	Number,			// 0..9
	Expression,		// a + a
	Object			// identifier
};



TypesOfValue GetTypeOfValue(std::string value) {
	if (System::Text::IsString(value)) return TypesOfValue::String;
	if (System::Text::IsChar(value)) return TypesOfValue::Char;
	if (System::Text::IsNumeric(value, TokenList::NumberSuffix)) return TypesOfValue::Number;
	// Math expression
	// Defines
	// Conditionnal condition expressions
}
