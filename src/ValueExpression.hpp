#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Object.hpp"
#include "MathExpression.hpp"
#include "Expression.hpp"

enum TypesOfValue {
	String,			// "..."
	Char,			// '...'
	Number,			// 0..9
	MathExpression, // a + b
	Expression,		// a to b | a == b ? a : b
	Object,			// identifier
	Nothing,
	Unknown
};

TypesOfValue GetTypeOfValue(Expr ExprValue) {
	std::string value;
	for (token_t token : ExprValue) value += token.value;
	if (Parser::TypeAlreadyExist(value)) return TypesOfValue::Object;
	if (System::Text::IsString(value)) return TypesOfValue::String;
	if (System::Text::IsChar(value)) return TypesOfValue::Char;
	if (System::Text::IsNumeric(value, TokenList::NumberSuffix)) return TypesOfValue::Number;
	
	Math::Expression math;
	if (math.IsMathExpression(ExprValue)) return TypesOfValue::MathExpression;
	// Conditionnal condition expressions / sequence / ...
	if (value.empty()) return TypesOfValue::Nothing;
	return TypesOfValue::Unknown;
}
