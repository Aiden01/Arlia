#pragma once
#include "System.hpp"
#include "Lexer.hpp"
#include "Assembler.hpp"
#include "Errors.hpp"
#include "Expression.hpp"

namespace Parser {
	class parser {
	private:

	public:
		void parse(Lexer&, size_t&, std::vector<std::string>&, Exception&, std::string = "");
	};
}
