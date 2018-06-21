#pragma once
#include "System.hpp"
#include "statements.h"
#include "identifier.hpp"

class Lexer {
public:
	struct analyse {
		keywords::keywords keyword;
		std::string value;
	};
private:
	std::vector<analyse> tokenized; // Contains all keywords' identifier as enum + values
public:
	Lexer(std::vector<std::string> tokens) {
		for (std::string token : tokens) {
			keywords::keywords Token = keywords::ToKeyword(token);
			tokenized.push_back({ keywords::ToKeyword(token), token });
			if (Token == keywords::keywords::UNKNOWN) LogMessage::ErrorMessage("Unknown token: " + token);
		}
	}
	std::vector<analyse> get() {
		return tokenized;
	}
};
