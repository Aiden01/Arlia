#pragma once
#include "System.hpp"

namespace MathExpressionAst {

	static std::string StrToExp("");

	class Exp {
	public:
		virtual void Display() {};
		virtual void Release() {};
		virtual void Build() {};
	};

	class Term : public Exp {
		std::string value;
	public:
		Term(std::string val) :value(val) {};
		void Display() {
			std::cout << ' ' << value << ' ';
		}
		void Build() {
			StrToExp += ' ' + value + ' ';
		}
		void Release() {};
	};

	class Node : public Exp {
		Exp *left;
		Exp *right;
		char operand;
	public:
		Node(char operand, Exp *left, Exp *right) :operand(operand), left(left), right(right) {};
		~Node() {};
		void Display() {
			std::cout << '(' << operand << ' ';
			left->Display();
			right->Display();
			std::cout << ')';
		}
		void Release() {
			left->Release();
			right->Release();
			delete left;
			delete right;
		}
		void Build() {
			StrToExp += "(" + System::Text::CharToString(operand) + " ";
			left->Build();
			right->Build();
			StrToExp += ")";
		}
	};

	inline Exp *StringToExpression(std::string str) {
		int level = 0;
		for (int i = str.size() - 1; i >= 0; --i) {
			char chr = str[i];
			if (chr == ')') {
				++level;
				continue;
			}
			if (chr == '(') {
				--level;
				continue;
			}
			if (level > 0) continue;
			if ((chr == '+' || chr == '-') && i != 0) {
				std::string left(str.substr(0, i));
				std::string right(str.substr(i + 1));
				return new Node(chr, StringToExpression(left), StringToExpression(right));
			}
		}
		for (int i = str.size() - 1; i >= 0; --i) {
			char chr = str[i];
			if (chr == ')') {
				++level;
				continue;
			}
			if (chr == '(') {
				--level;
				continue;
			}
			if (level > 0) continue;
			if (chr == '*' || chr == '/' || chr == '%') {
				std::string left(str.substr(0, i));
				std::string right(str.substr(i + 1));
				return new Node(chr, StringToExpression(left), StringToExpression(right));
			}
		}
		if (str[0] == '(') {
			for (int i = 0; i < str.size(); ++i) {
				if (str[i] == '(') {
					++level;
					continue;
				}
				if (str[i] == ')') {
					--level;
					if (level == 0) {
						std::string exp(str.substr(1, i - 1));
						return StringToExpression(exp);
					}
					continue;
				}
			}
		}
		else return new Term(str);
		// System::Display::ErrorMessage("never execute point");
		return NULL;
	}
};
