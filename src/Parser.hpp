#pragma once
#include "System.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Assembler.hpp"
#include "Errors.hpp"

namespace Parser {
	class parser {
	private:
		AST::AST ast;
	public:
		void parse(Lexer&, size_t&, std::vector<std::string>&, Exception&, std::string = "");
	};
}
