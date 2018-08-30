#pragma once
#include "System.hpp"
#include "LogMessage.hpp"
#include "TokenList.h"
#include "Expression.hpp"

class Exception {
public:
	// See file for explaination of error codes
	enum ErrorCodes {
		E0000,
		E0001, //  Can't load file
		E0002, //  Can't find header file
		E0003, //  Expected file name
		E0004, //  Unexpected token
		E0005, //  Expected Identifier
		E0006, //  Value without storage object
		E0007, //  Already existing object
		E0008, //  Redefined preprocessor definition
		E0009, //  Redefined variable
		E0010, //  Redefined function
		E0011, //  Redefined object
		E0012, //  Redefined rule
		E0013, //  Redefined value
		E0014, //  Undefined identifier
		E0015, //  Buoyage symbol missing
		E0016, //  Expected end of line symbol (';')
		E0017, //  Unexpected character
		E0018, //  Unknown token
		E0019, //  Non-close comment
		E0020, //  Non-compliant typing
		E0021, //  Use of uninitialized data
		E0022, //  Unacceptable suffix
		E0023, //  Invalid keyword combination
		E0024, //  Declaration not in accordance with non-local use
		E0025, //  Incompatible value with type / object defined
		E0026, //  Constant value too wide
		E0027, //  Too much initialization value in a static size array
		E0028, //  Non-conforming identifier
		E0029, //  Expected statement
		E0030, //  Wrong identifier format (the expected format is ASCII)
		E0031, //  Wrong pointer initialization
		E0032, //  Non-valid literal suffix
		E0033, //  No object can have the identifier 'vanaur'
		E0034, //  Non-existent object field
		E0035, //  Non-existent object method
		E0036, //  Missing function call arguments
		E0037, //  A rvalue cannot be accessed via the pointers
		E0038, //  The use of a pointer is unacceptable in this context
		E0039, //  A reference cannot be made to a constant value
		E0040, //  Invalid assembler code in external expression
		E0041, //  The types of the two objects do not match in the image assignment
		E0042, //  Only a defined object can be destroyed by the delete operator
		E0043, //  Empty template
		E0044, //  Object not existing in this namespace
		E0045, //  Typage temptation with a non-instanciated structure
		E0046, //  Temptation to access a private field of the object
		E0047, //  Temptation to access a private method of the object
		E0048, //  Invalid operator
		E0049, //  Reserved identifier
		E0050, //  Wrong usage of postfix and prefix operator
		E0051, //  Temptation to access an inaccessible element in a table
		E0052, //  The main function can only have 3 arguments variation
		E0053, //  This function is not defined as being able to return an array
		E0054, //  A non-instanciated structure cannot be used as an object instance with the keyword 'new'.
		E0055, //  Wrong choice of the anonymous string concatenation operator (between '^' and'+')
		E0056, //  An object can only inherit from one other object
		E0057, //  Incorrect 'typename' value
		E0058, //  Incorrect 'typesize' value
		E0059, //  The identifier of a process ('proc') cannot contain spaces
		E0060, //  The identifier of a process ('proc') cannot contain balises
		E0061, //  An exception cannot be thrown out of a function
		E0062, //  Syntax error
		E0063, //  The increment value of an iteration (with step) must be numerical
		E0064, //  An anonymous array serving as an iterator to a for loop cannot be empty
		E0065, //  Wrong ternary condition format
		E0066, //  A 'ret' instruction must only fit on one instruction (one line = one ';')
		E0067, //  Division by zero
		E0068, //  Wrong enumeration value
		E0069, //  'Upon' block members cannot contain expressions other than 'it'
		E0070, //  A match/case expression must contain at least one 'case' expression before using 'default'
		E0071, //  Unknown error
		E0072, //  Undefined object
		E0073, //  Not a valid math expression
		E0074, //  Expected character
		E0075, //  Unbalanced brackets
		E0076, //  Non-ascii character detected
		E0077, //  The current version of Arlia only accepts constant values as optional function parameters
		E0078, //  Undefined type
		E0079, //  Statement not recognised in this context
		E0080, //  Invalid statement
		E0081, //  Invalid statement / expression
		E0082, //  Attempt to edit an unsuitable token
	};
	void ThrowError(ErrorCodes, char);
	void ThrowError(ErrorCodes, std::string);
	void ThrowError(ErrorCodes, token_t);
	void ThrowError(ErrorCodes, token_t, Expr);
	void ThrowError(ErrorCodes, Expr);
private:
	std::map<ErrorCodes, std::string> Messages =
	{
		{ E0001, " Can't load file" },
	{ E0002, " Can't find header file" },
	{ E0003, " Expected file name" },
	{ E0004, " Unexpected token" },
	{ E0005, " Expected Identifier" },
	{ E0006, " Value without storage object" },
	{ E0007, " Already existing object" },
	{ E0008, " Redefined preprocessor definition" },
	{ E0009, " Redefined variable" },
	{ E0010, " Redefined function" },
	{ E0011, " Redefined object" },
	{ E0012, " Redefined rule" },
	{ E0013, " Redefined value" },
	{ E0014, " Undefined identifier" },
	{ E0015, " Buoyage symbol missing" },
	{ E0016, " Expected end of line symbol (';')" },
	{ E0017, " Unexpected character" },
	{ E0018, " Unknown token" },
	{ E0019, " Non-close comment" },
	{ E0020, " Non-compliant typing" },
	{ E0021, " Use of uninitialized data" },
	{ E0022, " Unacceptable suffix" },
	{ E0023, " Invalid keyword combination" },
	{ E0024, " Declaration not in accordance with non-local use" },
	{ E0025, " Incompatible value with type / object defined" },
	{ E0026, " Constant value too wide" },
	{ E0027, " Too much initialization value in a static size array" },
	{ E0028, " Non-conforming identifier" },
	{ E0029, " Expected statement" },
	{ E0030, " Wrong identifier format (the expected format is ASCII)" },
	{ E0031, " Wrong pointer initialization" },
	{ E0032, " Non-valid literal suffix" },
	{ E0033, " No object can have the identifier 'vanaur'" },
	{ E0034, " Non-existent object field" },
	{ E0035, " Non-existent object method" },
	{ E0036, " Missing function call arguments" },
	{ E0037, " A rvalue cannot be accessed via the pointers" },
	{ E0038, " The use of a pointer is unacceptable in this context" },
	{ E0039, " A reference cannot be made to a constant value" },
	{ E0040, " Invalid assembler code in external expression" },
	{ E0041, " The types of the two objects do not match in the image assignment" },
	{ E0042, " Only a defined object can be destroyed by the delete operator" },
	{ E0043, " Empty template" },
	{ E0044, " Object not existing in this namespace" },
	{ E0045, " Typage temptation with a non-instanciated structure" },
	{ E0046, " Temptation to access a private field of the object" },
	{ E0047, " Temptation to access a private method of the object" },
	{ E0048, " Invalid operator" },
	{ E0049, " Reserved identifier" },
	{ E0050, " Wrong usage of postfix and prefix operator" },
	{ E0051, " Temptation to access an inaccessible element in a table" },
	{ E0052, " The main function can only have 3 arguments variation" },
	{ E0053, " This function is not defined as being able to return an array" },
	{ E0054, " A non-instanciated structure cannot be used as an object instance with the keyword 'new'" },
	{ E0055, " Wrong choice of the anonymous string concatenation operator (between '^' and'+')" },
	{ E0056, " An object can only inherit from one other object" },
	{ E0057, " Incorrect 'typename' value" },
	{ E0058, " Incorrect 'typesize' value" },
	{ E0059, " The identifier of a process ('proc') cannot contain spaces" },
	{ E0060, " The identifier of a process ('proc') cannot contain balises" },
	{ E0061, " An exception cannot be thrown out of a function" },
	{ E0062, " Syntax error" },
	{ E0063, " The increment value of an iteration (with step) must be numerical" },
	{ E0064, " An anonymous array serving as an iterator to a for loop cannot be empty" },
	{ E0065, " Wrong ternary condition format" },
	{ E0066, " A 'ret' instruction must only fit on one instruction (one line = one ';')" },
	{ E0067, " Division by zero" },
	{ E0068, " Wrong enumeration value" },
	{ E0069, " 'Upon' block members cannot contain expressions other than 'it'" },
	{ E0070, " A match/case expression must contain at least one 'case' expression before using 'default'" },
	{ E0071, " Unknown error" },
	{ E0072, " Undefined object" },
	{ E0073, " Not a valid math expression" },
	{ E0074, " Expected character" },
	{ E0075, " Unbalanced brackets" },
	{ E0076, " Non-ascii character detected" },
	{ E0077, " The current version of Arlia only accepts constant values as optional function parameters" },
	{ E0078, " Undefined type" },
	{ E0079, " Statement not recognised in this context" },
	{ E0080, " Invalid statement" },
	{ E0081, " Invalid statement or expression" },
	{ E0082, " Attempt to edit an unsuitable token" },

	{ E0000, " ... " }
	};
};
