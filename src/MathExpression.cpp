#include "MathExpression.hpp"
#include "VariableDecl.hpp"
#include "AssemblerMacros.hpp"
#include "Expression.hpp"
#define ErrorStr "\r"

template<int index>
std::string VariableValueAdress(std::vector<std::string> &terms) {
	if (index > terms.size() || index < 0) return ErrorStr;
	if (TokenList::IsIdentifier(terms[index])) return "[" + Parser::GetVariable(terms[index]).Adress + "]";
	return terms[index];
}

bool Math::Expression::IsMathExpression(Expr expr, bool ShowErr) {
	bool ret = true;
	std::string expression;

	for (size_t i = 0; i < expr.size(); ++i) {
		token_t bef, current, next;
		if (i > 0) bef = expr[i - 1];
		if (i < expr.size() - 1) next = expr[i + 1];
		current = expr[i];

		if (current.type == TokenList::UNKNOWN) {
			if (ShowErr) exception.ThrowError(this->exception.E0018, current, expr);
			ret = false;
		}
		if (current.type == TokenList::NUMBER && bef.type == TokenList::NUMBER) {
			if (ShowErr) this->exception.ThrowError(this->exception.E0004, expr[i], expr);
			ret = false;
		}
		if (!current.type == TokenList::NUMBER && !current.type == TokenList::IDENTIFIER && !System::Math::IsMathSymbol(current.value) && current.value != ")" && current.value != "(") {
			if (ShowErr) this->exception.ThrowError(this->exception.E0004, expr[i], expr);
			ret = false;
		}
		if (TokenList::IsKeyword(current.value)) {
			if (ShowErr) this->exception.ThrowError(exception.E0004, current, expr);
			ret = false;
		}
		if (System::Math::IsMathSymbol(current.value) && next.value.empty()) {
			if (current.value == "++" || current.value == "--") break;
			if (ShowErr) this->exception.ThrowError(exception.E0004, current, expr);
			ret = false;
		}
		if (System::Math::IsMathSymbol(current.value) && System::Math::IsMathSymbol(bef.value)) {
			if (bef.value == "++" || bef.value == "--") break;
			if (!((current.value == "++" && bef.value == "++") || (current.value == "--" && bef.value == "--"))) {
				if (ShowErr) exception.ThrowError(this->exception.E0004, current, expr);
				ret = false;
			}
		}
		if (current.value == "++" || current.value == "--") {
			if (bef.type == TokenList::NUMBER || bef.type == TokenList::IDENTIFIER &&
				next.type == TokenList::NUMBER || next.type == TokenList::IDENTIFIER
				) {
				if (ShowErr) this->exception.ThrowError(exception.E0004, expr[i], expr);
				ret = false;
			}
		}
		if ((current.value == "/" || current.value == "%" ) && System::Text::DeleteUnnecessaryZeros(next.value) == "0") {
			if (ShowErr) this->exception.ThrowError(this->exception.E0067, expr[i + 1], expr);
			ret = false;
		}
		expression += current.value;
	}
	if (!System::Text::AreParanthesesBalanced(expression)) {
		if (ShowErr) this->exception.ThrowError(this->exception.E0074, expr);
		ret = false;
	}
	if (!ret) if (ShowErr) this->exception.ThrowError(this->exception.E0073, expr);
	return ret;
}
bool Math::Expression::CanInterpret(Expr expr) {
	// Suffixes!
	for (token_t token : expr) if (token.type == TokenList::TokenList::IDENTIFIER) return false;
	return true;
}

std::string Math::Expression::TakeASTexp(std::string expression) {
	MathExpressionAst::Exp *tree = MathExpressionAst::StringToExpression(expression);
	tree->Build();
	tree->Release();
	delete tree;
	std::string ret = System::Text::ReplaceMultiSpace(MathExpressionAst::StrToExp);
	MathExpressionAst::StrToExp.clear();
	return ret;
}
std::string Math::Expression::MathOperatorToAsmInstruction(std::string symb) {
	std::map<std::string, std::string> ConvertOperators =
	{ {"+", "add"}, {"-", "sub"}, {"*", "imul"}, {"/", "div"}, {"%", "mod"}, {"++", "inc"}, {"--", "dec"} };
	return ConvertOperators[symb];
}
std::string Math::Expression::Interpret(Expr expr) {
	std::string result;
	for (token_t token : expr) result += token.value;
	result = std::to_string(System::Math::evaluate(result));
	return System::Text::DeleteUnnecessaryZeros(result);
}
std::string Math::Expression::ConvertToAsm(Expr expr) {
	std::string expression;
	for (token_t token : expr) expression += token.value;
	std::string ast = this->TakeASTexp(expression), temp, ret;

	std::vector<std::string> vec = System::Text::EachItemInDelimiters(ast, '(', ')');

	// Variables -> adresses !

	for (size_t i = 0; i < vec.size(); ++i) {
		std::string branch = System::Text::ReplaceMultiSpace(vec[i].substr(1, vec[i].size() - 2)), adress;
		std::vector<std::string> terms = System::Vector::split(branch, " ");

		adress = VariableValueAdress<1>(terms);

		if (adress == "") adress = std::to_string(i);

		if (i == 0) {
			temp += "mov " + this->reg + ", " + adress + "\n";
			adress = VariableValueAdress<2>(terms);
		}

		if (adress == "") adress = std::to_string(i);

		if (terms[0] == "%") temp += Assembler::Macro::UseModulo(this->reg, adress) + " mov " + this->reg + ", ebx\n";
		else temp += this->MathOperatorToAsmInstruction(terms[0]) + " " + this->reg + ", " + adress + "\n";

	}

	std::vector<std::string> VecLines = System::Text::GetEachLine(temp);

	for (size_t i = 0; i < VecLines.size(); ++i) {
		ret += "\t";
		if (i == 0) ret += VecLines[i] + "\n";
		else if (!System::Text::StartsWih(VecLines[i], "mov ")) ret += VecLines[i] + "\n";
	}

	return ret;
}
