#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Errors.hpp"

#define HeaderExt ".k"

void ImportFailed(std::vector<token_t>, std::string, Exception&, bool&, bool);
bool CanImport(std::vector<token_t>);
std::string GetImportFilename(std::vector<token_t>);

class Lexer {
private:
	Exception exception;
	std::string filename;
	std::ifstream File;
	int CurrentCharIndex = 0, LineIndex = 1, CurrentCharInCurrentLineIndex = -1;
public:
	Lexer(std::string filename, bool StopAferFirstError);
	Lexer() { }; // Default constructor
	void start(std::string filename);
	void end();
	bool CanContinue = false; // if false: stop compiler
	bool eof = false;
	bool StopAferFirstError = false;
	int NbrOfTokens;
	char peekchr();
	token_t next();
	std::vector<token_t> GetLine();
};
