#pragma once
#include "System.hpp"
#include "MathAst.hpp"
#include "TokenList.h"
#include "Assembler.hpp"
#include "Errors.hpp"
#include "VariableDecl.hpp"

namespace Math {

	class Expression {
	private:
		std::string reg = "edi";
		Exception exception;
		std::vector<token_t> expression;

		std::string TakeASTexp(std::string expression);
		std::string MathOperatorToAsmInstruction(std::string symb);

	public:
		void ShowAst(std::string str) {
			MathExpressionAst::Exp *tree = MathExpressionAst::StringToExpression(str);
			tree->Build();
			tree->Display();
		}
		bool IsMathExpression(std::vector<token_t>, bool = true);
		bool CanInterpret(std::vector<token_t>);
		std::string Interpret(std::vector<token_t>);
		std::string ConvertToAsm(std::vector<token_t>);
	};
}
