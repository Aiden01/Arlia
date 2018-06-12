#pragma once
#include "System.hpp"
#include "statements.h"
#include "identifier.hpp"

class Lexer {
private:
	struct analyse {
		keywords::keywords keyword;
		std::string value;
	};
	std::vector<analyse> tokenized; // Contains all keywords' identifier as enum + values
public:
	Lexer(std::vector<std::string> tokens) {
		for (std::string token : tokens)
			tokenized.push_back({ keywords::ToKeyword(token), token });

		for (analyse item : tokenized)
			std::cout << item.keyword << " -> " << item.value << std::endl;
	}
};
