#pragma once
#include "System.hpp"
#include "TokenList.h"

#define HeaderExt ".k"

void ImportFailed(std::vector<token_t>, std::string, bool&);
bool CanImport(std::vector<token_t>);
std::string GetImportFilename(std::vector<token_t>);

class Lexer {
private:
	std::string filename;
	std::ifstream File;
	int CurrentCharIndex = 0, LineIndex = 1, CurrentCharInCurrentLineIndex = -1;
public:
	Lexer(std::string filename);
	Lexer() { }; // Default constructor
	void start(std::string filename);
	void end();
	bool CanContinue = false; // if false: stop compiler
	bool eof = false;
	int NbrOfTokens;
	char peekchr();
	token_t next();
	std::vector<token_t> GetLine();
};
