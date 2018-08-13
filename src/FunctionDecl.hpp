#pragma once
#include "System.hpp"
#include "Location.hpp"
#include "Object.hpp"
#include "Errors.hpp"
#include "TokenList.h"
#include "Expression.hpp"
#include "Assembler.hpp"
#include "LL_NameConventions.hpp"

namespace Parser {

	struct Argument {
		std::string Identifier;
		type_t Type;
		std::string Value;
		bool IsPointer;  // ex: var MyRef = &MyVar; ~MyRef = 9;
	};
	struct FunctionDecl_t {
		std::string Identifier;
		type_t Type;
		std::vector<Argument> Arguments;
		bool IsArray;
		bool IsPrivate;
		location_t Position;
	};

	typedef FunctionDecl_t function_t;

	class Function {
	private:
		std::vector<FunctionDecl_t> ListOfFunctions;
	public:
		bool IsFunctionDeclaration(Expr expr) {
			return expr[0].type == TokenList::TokenList::FUNC;
		}
		bool IsAcceptableFunctionDeclaration(Expr expr) {
			bool ret = true;
			Exception exception;
			std::string tmp;
			for (size_t i = 0; i < expr.size(); ++i) tmp += expr[i].value;
			if (!System::Text::AreParanthesesBalanced(tmp)) {
				exception.ThrowError(exception.E0075, expr);
				ret = false;
				goto end;
			}
			for (size_t i = 0; i < expr.size(); ++i) {
				// function identifier
				if (i == 1) if (expr[i].type != TokenList::IDENTIFIER) {
					exception.ThrowError(exception.E0028, expr[i]);
					ret = false;
				}
				if (i == 1) if (FunctionAlreadyExist(ConvertToLL(expr[i].value))) {
					exception.ThrowError(exception.E0010, expr[i]);
					ret = false;
				}
				// arguments
				if (i == 2) if (expr[i].type != TokenList::LEFT_PARENTHESIS) {
					exception.ThrowError(exception.E0004, expr[i]);
					ret = false;
					goto end; // can't analyse rest
				}
				if (i == 3) if (expr[i].type != TokenList::RIGHT_PARENTHESIS) {
					int j = 0;
					while (expr[i].type != TokenList::RIGHT_PARENTHESIS || i == expr.size()) {
						if (expr[i].type == TokenList::COMMA) {
							j = 0;
							++i;
							continue;
						}
						if (j == 0) if (expr[i].type != TokenList::IDENTIFIER) {
							exception.ThrowError(exception.E0028, expr[i]);
							ret = false;
						}
						if (j == 1) if (expr[i].type != TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) {
							if (expr[i].type != TokenList::EQUAL) {
								exception.ThrowError(exception.E0004, expr[i]);
								ret = false;
							}
						}
						if (j == 2) if (expr[i - 1].type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) {
							if (!Parser::TypeAlreadyExist(ConvertToLL(expr[i].value))) {
								exception.ThrowError(exception.E0078, expr[i]);
								ret = false;
							}
						}
						if (j == 3) if (expr[i].type != TokenList::EQUAL) {
							exception.ThrowError(exception.E0004, expr[i]);
							ret = false;
						}
						else if (expr[i - 1].type == TokenList::EQUAL) {
							if (
								expr[i].type != TokenList::NUMBER &&
								expr[i].type != TokenList::CHAR &&
								expr[i].type != TokenList::STRING &&
								expr[i].type != TokenList::BOOLEAN
								) {
								exception.ThrowError(exception.E0077, expr[i]);
								ret = false;
							}
						}
						else if (expr[i].type != TokenList::EQUAL) {
							exception.ThrowError(exception.E0004, expr[i]);
							ret = false;
						}
						if (j > 4) {
							exception.ThrowError(exception.E0004, expr[i]);
							ret = false;
						}
						++i;
						++j;
					}
				}
				else if (expr[i].type != TokenList::RIGHT_PARENTHESIS) {
					exception.ThrowError(exception.E0004, expr[i]);
					ret = false;
				}
				// type of function
				if (i > 3) {
					if (expr[i - 1].type == TokenList::LEFT_HOOK && expr[i].type != TokenList::RIGHT_HOOK) {
						exception.ThrowError(exception.E0004, expr[i]);
						ret = false;
					}
					else if (expr[i - 1].type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL && !Parser::TypeAlreadyExist(ConvertToLL(expr[i].value))) {
						exception.ThrowError(exception.E0078, expr[i]);
						ret = false;
					}
					else if (expr[i].type != TokenList::RIGHT_PARENTHESIS && expr[i].type != TokenList::RIGHT_HOOK && expr[i].type != TokenList::LEFT_HOOK && expr[i].type != TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL && expr[i - 1].type != TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) {
						exception.ThrowError(exception.E0004, expr[i]);
						ret = false;
					}
				}
			}
		end:
			return ret;
		}
		FunctionDecl_t ExprToFuncDecl(Expr expr) {
			FunctionDecl_t ret;
			ret.Identifier = ConvertToLL(expr[1].value);
			std::vector<Argument> arguments;
			for (size_t i = 0; i < expr.size(); ++i) {
				if (i == 3) if (expr[i].type != TokenList::RIGHT_PARENTHESIS) {
					size_t j = 0;
					while (expr[i].type != TokenList::RIGHT_PARENTHESIS || i == expr.size()) {
						Argument arg;
						if (expr[i].type == TokenList::COMMA) {
							j = 0;
							++i;
							continue;
						}
						if (j == 0) arg.Identifier = expr[i].value;
						if (expr[i - 1].type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) arg.Type = Parser::GetType(ConvertToLL(expr[i].value));
						++i;
						++j;
						arguments.push_back(arg);
					}
					ret.Arguments = arguments;
				}
				// type of function
				if (i > 3) {
					if (expr[i - 1].type == TokenList::LEFT_HOOK && expr[i].type == TokenList::RIGHT_HOOK) ret.IsArray = true;
					else ret.IsArray = false;
					if (expr[i - 1].type == TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL) ret.Type = Parser::GetType(ConvertToLL(expr[i].value));
				}
			}
			return ret;
		}
		void AddFunction(FunctionDecl_t decl) {
			ListOfFunctions.push_back(decl);
		}
		bool FunctionAlreadyExist(std::string identifier) {
			for (std::vector<FunctionDecl_t>::iterator it = ListOfFunctions.begin(); it != ListOfFunctions.end(); ++it)
				if (it->Identifier == identifier) return true;
			return false;
		}
		function_t Get(std::string identifier) {
			for (std::vector<FunctionDecl_t>::iterator it = ListOfFunctions.begin(); it != ListOfFunctions.end(); ++it)
				if (it->Identifier == identifier) return ListOfFunctions[it - ListOfFunctions.begin()];
			return {};
		}
	};

}
