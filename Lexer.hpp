#pragma once
#include "System.hpp"
#include "TokenList.h"

class Lexer {
private:
	std::string SourceFilename, CurrentFilename;
	std::ifstream File, Header;
	bool IsInHeader = false;
	int CurrentCharIndex = 0, SourceLineIndex = 1, HeaderLineIndex = 1, CurrentCharInCurrentLineIndex = -1;
public:
	Lexer(std::string filename);
	bool CanContinue = false; // if false: stop compiler
	bool eof = false;
	bool CanImport(std::vector<token_t> line);
	void import(std::string filename);
	int NbrOfTokens;
	char peek();
	token_t next();
	std::vector<token_t> GetLine();
};
