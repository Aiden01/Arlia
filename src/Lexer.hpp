#pragma once
#include "System.hpp"
#include "TokenList.h"

class Lexer {
private:
	std::string CurrentFilename;
	std::ifstream File;
	size_t Max = 0; // Size of file
	int CurrentCharIndex = 0;
	int CurrentLineIndex = 1;
	int CurrentCharInCurrentLineIndex = -1; // Last char position of token
	bool IsImportSyntax(std::vector<token_t> line);
public:
	Lexer(std::string filename);
	bool CanContinue = false; // if false: stop compiler
	bool eof = false;
	int NbrOfTokens;
	token_t next();
	std::vector<token_t> GetLine();
};
