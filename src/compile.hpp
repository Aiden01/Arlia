#pragma once
#include <time.h>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Assembler.hpp"

class Compile {
private:
	Exception exception;
	Lexer lexer;
	Parser::parser parser;
	std::string SourceFile, OutputAsm, OutputExe;
	std::vector<std::string> HeaderImported;
	Assembler::AsmFinalCode code;
	clock_t tclock;
	double time;
	void SetFilenames();
	std::string FileSize(std::string filename);
public:
	Compile(std::string filename);
	void run();
	bool HasErrors();
	double GetTime();
	void write();
	bool IsSuccess();
	void DisplayCompilationInformation();
};
