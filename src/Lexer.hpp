#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Errors.hpp"
#include "Expression.hpp"

#define HeaderExt ".k"

void ImportFailed(Expr, std::string, Exception&, bool&, bool);
bool CanImport(Expr);
std::string GetImportFilename(Expr);

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
	Expr GetLine();
	Expr GetUntil(TokenList::TokenList until, bool included);
};
