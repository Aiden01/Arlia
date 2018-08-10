#pragma once
#include "System.hpp"
#include "MathAst.hpp"
#include "TokenList.h"
#include "Assembler.hpp"
#include "Errors.hpp"
#include "VariableDecl.hpp"
#include "Expression.hpp"

namespace Math {

	class Expression {
	private:
		std::string reg = "edi";
		Exception exception;
		Expr expression;

		std::string TakeASTexp(std::string expression);
		std::string MathOperatorToAsmInstruction(std::string symb);

	public:
		void ShowAst(std::string str) {
			MathExpressionAst::Exp *tree = MathExpressionAst::StringToExpression(str);
			tree->Build();
			tree->Display();
		}
		bool IsMathExpression(Expr, bool = true);
		bool CanInterpret(Expr);
		std::string Interpret(Expr);
		std::string ConvertToAsm(Expr);
	};
}
