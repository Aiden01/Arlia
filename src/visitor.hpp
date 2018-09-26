#pragma once
#include "System.hpp"
#include "Parser.hpp"

// classes
#include "identifier.hpp"
// <-- classes

/// https://openclassrooms.com/forum/sujet/compilateur-clang-ast?page=1#message-92597455

namespace Parser {

	class Node;
	class BinaryNode;
	class UnaryNode;
	class Leaf;

	class Visitor {
	public:
		// statements
		virtual void visit(class var_stmt&) = 0;					// var
		virtual void visit(class ret_stmt&) = 0;					// ret
		virtual void visit(class func_stmt&) = 0;					// func
		virtual void visit(class instance_stmt&) = 0;				// instance
		virtual void visit(class structure_stmt&) = 0;				// structure
		virtual void visit(class object_stmt&) = 0;					// instance structure
		virtual void visit(class rule_stmt&) = 0;					// instance ret
		virtual void visit(class for_stmt&) = 0;					// for
		virtual void visit(class while_stmt&) = 0;					// while
		virtual void visit(class enum_stmt&) = 0;					// enum
		virtual void visit(class namespace_stmt&) = 0;				// namespace
		virtual void visit(class return_stmt&) = 0;					// return
		virtual void visit(class continue_stmt&) = 0;				// continue
		virtual void visit(class if_stmt&) = 0;						// if
		virtual void visit(class elif_stmt&) = 0;					// elif
		virtual void visit(class else_stmt&) = 0;					// else
		virtual void visit(class externe_stmt&) = 0;				// externe
		virtual void visit(class match_stmt&) = 0;					// match
		virtual void visit(class case_stmt&) = 0;					// case
		virtual void visit(class default_stmt&) = 0;				// default
		virtual void visit(class throw_stmt&) = 0;					// throw
		virtual void visit(class try_stmt&) = 0;					// try
		virtual void visit(class implement_stmt&) = 0;				// implement
		virtual void visit(class public_stmt&) = 0;					// public
		virtual void visit(class private_stmt&) = 0;				// private
		virtual void visit(class static_stmt&) = 0;					// static
		virtual void visit(class upon_stmt&) = 0;					// upon
		virtual void visit(class set_stmt&) = 0;					// set
		// keywords operators
		virtual void visit(class is_op&) = 0;						// is
		virtual void visit(class isnt_op&) = 0;						// isnt
		virtual void visit(class each_op&) = 0;						// each
		virtual void visit(class to_op&) = 0;						// to
		virtual void visit(class in_op&) = 0;						// in
		virtual void visit(class step_op&) = 0;						// step
		virtual void visit(class new_op&) = 0;						// new
		virtual void visit(class delete_op&) = 0;					// delete
		virtual void visit(class free_op&) = 0;						// free		
		virtual void visit(class it_op&) = 0;						// it
		// symbole operators - math
		virtual void visit(class plus_op&) = 0;						// +
		virtual void visit(class minus_op&) = 0;					// -
		virtual void visit(class time_op&) = 0;						// *
		virtual void visit(class divide_op&) = 0;					// /
		virtual void visit(class modulo_op&) = 0;					// %
		virtual void visit(class power_op&) = 0;					// **
		virtual void visit(class increment_op&) = 0;				// ++
		virtual void visit(class decrement_op&) = 0;				// --
		// symbole operators - array
		virtual void visit(class AnonymeConcat_op&) = 0;			// ^
		virtual void visit(class PlusConcat_op&) = 0;				// ^+
		virtual void visit(class MinusConcat_op&) = 0;				// ^-
		virtual void visit(class TimeConcat_op&) = 0;				// ^*
		virtual void visit(class DivideConcat_op&) = 0;				// ^/
		virtual void visit(class ModuloConcat_op&) = 0;				// ^%
		// symbole operators - assign
		virtual void visit(class equal_op&) = 0;					// =
		virtual void visit(class PlusEqual_op&) = 0;				// +=
		virtual void visit(class MinusEqual_op&) = 0;				// -=
		virtual void visit(class TimeEqual_op&) = 0;				// *=
		virtual void visit(class DivideEqual_op&) = 0;				// /=
		virtual void visit(class ModuloEqual_op&) = 0;				// %=
		// symbole operators - conditionnal
		virtual void visit(class or_op&) = 0;						// ||
		virtual void visit(class and_op&) = 0;						// &&
		virtual void visit(class IsEqual_op&) = 0;					// ==
		virtual void visit(class IsNotEqual_op&) = 0;				// !=
		virtual void visit(class opposite_op&) = 0;					// !
		virtual void visit(class IsLesser_op&) = 0;					// <
		virtual void visit(class IsGreater_op&) = 0;				// >
		virtual void visit(class IsLesserEQ_op&) = 0;				// <=
		virtual void visit(class IsGreaterEQ_op&) = 0;				// >=
		virtual void visit(class ternary_op&) = 0;					// ?
		virtual void visit(class TernaryValSep_op&) = 0;			// :
		// symboles - groups
		virtual void visit(class LeftHook_op&) = 0;					// [
		virtual void visit(class RightHook_op&) = 0;				// ]
		virtual void visit(class LeftBrace_op&) = 0;				// {
		virtual void visit(class RightBrace_op&) = 0;				// }
		virtual void visit(class LeftParenthesis_op&) = 0;			// (
		virtual void visit(class RightParenthesis_op&) = 0;			// )
		// symboles - miscellaneous
		virtual void visit(class dot_op&) = 0;						// .
		virtual void visit(class NamespaceCall_op&) = 0;			// @
		virtual void visit(class comma_op&) = 0;					// ,
		virtual void visit(class endline_op&) = 0;					// ;
		virtual void visit(class PointerAdress_op&) = 0;			// &
		virtual void visit(class PointerValue_op&) = 0;				// ~
		virtual void visit(class ObjectTyping&) = 0;				// :
		virtual void visit(class image_op&) = 0;					// -->
		// datas type
		virtual void visit(class number_data&) = 0;					// ..., 0, 1, 2, ...
		virtual void visit(class char_data&) = 0;					// '...'
		virtual void visit(class string_data&) = 0;					// "..."
		virtual void visit(class boolean_data&) = 0;				// true - false
		virtual void visit(class object&) = 0;						// ID
		virtual void visit(class array_data&) = 0;					// []..[]..[]
		// identifier
		virtual void visit(Identifier&) = 0;
		// template
		virtual void visit(class _typesize&) = 0;					// typesize
		virtual void visit(class _typename&) = 0;					// typename
		virtual void visit(class TemplateStart&) = 0;				// <
		virtual void visit(class TemplateEnd&) = 0;					// >

	protected:
		Visitor() = default;
		Visitor(Visitor const&) = delete;
		Visitor &operator=(Visitor const &) = delete;
		~Visitor() = default;
	};

	class ConstVisitor {
		// statements
		virtual void visit(class var_stmt const &) = 0;				// var
		virtual void visit(class ret_stmt const &) = 0;				// ret
		virtual void visit(class func_stmt const &) = 0;			// func
		virtual void visit(class instance_stmt const &) = 0;		// instance
		virtual void visit(class structure_stmt const &) = 0;		// structure
		virtual void visit(class object_stmt const &) = 0;			// instance structure
		virtual void visit(class rule_stmt const &) = 0;			// instance ret
		virtual void visit(class for_stmt const &) = 0;				// for
		virtual void visit(class while_stmt const &) = 0;			// while
		virtual void visit(class enum_stmt const &) = 0;			// enum
		virtual void visit(class namespace_stmt const &) = 0;		// namespace
		virtual void visit(class return_stmt const &) = 0;			// return
		virtual void visit(class continue_stmt const &) = 0;		// continue
		virtual void visit(class if_stmt const &) = 0;				// if
		virtual void visit(class elif_stmt const &) = 0;			// elif
		virtual void visit(class else_stmt const &) = 0;			// else
		virtual void visit(class externe_stmt const &) = 0;			// externe
		virtual void visit(class match_stmt const &) = 0;			// match
		virtual void visit(class case_stmt const &) = 0;			// case
		virtual void visit(class default_stmt const &) = 0;			// default
		virtual void visit(class throw_stmt const &) = 0;			// throw
		virtual void visit(class try_stmt const &) = 0;				// try
		virtual void visit(class implement_stmt const &) = 0;		// implement
		virtual void visit(class public_stmt const &) = 0;			// public
		virtual void visit(class private_stmt const &) = 0;			// private
		virtual void visit(class static_stmt const &) = 0;			// static
		virtual void visit(class upon_stmt const &) = 0;			// upon
		virtual void visit(class set_stmt const &) = 0;				// set
		// keywords operators
		virtual void visit(class is_op const &) = 0;				// is
		virtual void visit(class isnt_op const &) = 0;				// isnt
		virtual void visit(class each_op const &) = 0;				// each
		virtual void visit(class to_op const &) = 0;				// to
		virtual void visit(class in_op const &) = 0;				// in
		virtual void visit(class step_op const &) = 0;				// step
		virtual void visit(class new_op const &) = 0;				// new
		virtual void visit(class delete_op const &) = 0;			// delete
		virtual void visit(class free_op const &) = 0;				// free		
		virtual void visit(class it_op const &) = 0;				// it
		// symbole operators - math
		virtual void visit(class plus_op const &) = 0;				// +
		virtual void visit(class minus_op const &) = 0;				// -
		virtual void visit(class time_op const &) = 0;				// *
		virtual void visit(class divide_op const &) = 0;			// /
		virtual void visit(class modulo_op const &) = 0;			// %
		virtual void visit(class power_op const &) = 0;				// **
		virtual void visit(class increment_op const &) = 0;			// ++
		virtual void visit(class decrement_op const &) = 0;			// --
		// symbole operators - array
		virtual void visit(class AnonymeConcat_op const &) = 0;		// ^
		virtual void visit(class PlusConcat_op const &) = 0;		// ^+
		virtual void visit(class MinusConcat_op const &) = 0;		// ^-
		virtual void visit(class TimeConcat_op const &) = 0;		// ^*
		virtual void visit(class DivideConcat_op const &) = 0;		// ^/
		virtual void visit(class ModuloConcat_op const &) = 0;		// ^%
		// symbole operators - assign
		virtual void visit(class equal_op const &) = 0;				// =
		virtual void visit(class PlusEqual_op const &) = 0;			// +=
		virtual void visit(class MinusEqual_op const &) = 0;		// -=
		virtual void visit(class TimeEqual_op const &) = 0;			// *=
		virtual void visit(class DivideEqual_op const &) = 0;		// /=
		virtual void visit(class ModuloEqual_op const &) = 0;		// %=
		// symbole operators - conditionnal
		virtual void visit(class or_op const &) = 0;				// ||
		virtual void visit(class and_op const &) = 0;				// &&
		virtual void visit(class IsEqual_op const &) = 0;			// ==
		virtual void visit(class IsNotEqual_op const &) = 0;		// !=
		virtual void visit(class opposite_op const &) = 0;			// !
		virtual void visit(class IsLesser_op const &) = 0;			// <
		virtual void visit(class IsGreater_op const &) = 0;			// >
		virtual void visit(class IsLesserEQ_op const &) = 0;		// <=
		virtual void visit(class IsGreaterEQ_op const &) = 0;		// >=
		virtual void visit(class ternary_op const &) = 0;			// ?
		virtual void visit(class TernaryValSep_op const &) = 0;		// :
		// symboles - groups
		virtual void visit(class LeftHook_op const &) = 0;			// [
		virtual void visit(class RightHook_op const &) = 0;			// ]
		virtual void visit(class LeftBrace_op const &) = 0;			// {
		virtual void visit(class RightBrace_op const &) = 0;		// }
		virtual void visit(class LeftParenthesis_op const &) = 0;	// (
		virtual void visit(class RightParenthesis_op const &) = 0;	// )
		// symboles - miscellaneous
		virtual void visit(class dot_op const &) = 0;				// .
		virtual void visit(class NamespaceCall_op const &) = 0;		// @
		virtual void visit(class comma_op const &) = 0;				// ,
		virtual void visit(class endline_op const &) = 0;			// ;
		virtual void visit(class PointerAdress_op const &) = 0;		// &
		virtual void visit(class PointerValue_op const &) = 0;		// ~
		virtual void visit(class ObjectTyping const &) = 0;			// :
		virtual void visit(class image_op const &) = 0;				// -->
		// datas type
		virtual void visit(class number_data const &) = 0;			// ..., 0, 1, 2, ...
		virtual void visit(class char_data const &) = 0;			// '...'
		virtual void visit(class string_data const &) = 0;			// "..."
		virtual void visit(class boolean_data const &) = 0;			// true - false
		virtual void visit(class object const &) = 0;				// ID
		virtual void visit(class array_data const &) = 0;			// []..[]..[]
		// identifier
		virtual void visit(Identifier const &) = 0;
		// template
		virtual void visit(class _typesize const &) = 0;			// typesize
		virtual void visit(class _typename const &) = 0;			// typename
		virtual void visit(class TemplateStart const &) = 0;		// <
		virtual void visit(class TemplateEnd const &) = 0;			// >

	protected:
		ConstVisitor() = default;
		ConstVisitor(ConstVisitor const&) = delete;
		ConstVisitor &operator=(ConstVisitor const &) = delete;
		~ConstVisitor() = default;
	};
}
