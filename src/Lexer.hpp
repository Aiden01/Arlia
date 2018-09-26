#pragma once
#include "System.hpp"
#include "TokenList.h"
#include "Errors.hpp"
#include "Expression.hpp"
#include "DefineStmt.hpp"

#define HeaderExt ".k"

void ImportFailed(std::string HeaderFilename, location_t infos, Exception &exception);
bool CanImport(std::string HeaderFilename);
std::string GetImportFilename(Expr);

class Lexer {
private:
	Exception exception;
	std::string filename;
	std::ifstream File;
	Expr DefineValue;
	bool IsInDefine;
	int CurrentCharIndex = 0, LineIndex = 1, CurrentCharInCurrentLineIndex = 0;
	void Initialize(std::ifstream &File, std::string &filename_ref, std::string filename, bool &CanContinue, bool &eof, Exception &ex);
public:
	Lexer() {}; // Default constructor
	Lexer(std::string filename);
	Parser::DefineStatement Define;
	void start(std::string filename);
	void end();
	bool CanContinue = false; // if false: stop compiler
	bool eof = false;
	bool StopAferFirstError = false;
	char peekchr();
	token_t snext; // If set, is necessarily the next token to be returned.
	bool Issnext = false;
	token_t next();
	token_t peekt();
	Expr GetLine(bool keepit = true);
	Expr GetUntil(TokenList::TokenList until, bool included);
};
