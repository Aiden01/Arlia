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
	static bool IsFileIdentifier(std::string token) {
		return (System::Text::StartsWih(token, "<") &&
			System::Text::EndsWith(token, ">") &&
			System::File::exist(token.substr(1, token.length() - 2)));
	}
	Lexer(std::vector<std::string> tokens) {
		for (std::string token : tokens) {
			keywords::keywords Token = keywords::ToKeyword(token);
			tokenized.push_back({ keywords::ToKeyword(token), token });
			if (Token == keywords::keywords::UNKNOWN && !IsFileIdentifier(token))
				LogMessage::ErrorMessage("Unknown token: '" + token + "'");
		}
	}
	std::vector<analyse> get() {
		return tokenized;
	}
};
