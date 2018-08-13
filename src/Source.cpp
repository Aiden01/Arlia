#include "System.hpp"
#include "LogMessage.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "Assembler.hpp"

#include "tast.hpp"

#include <time.h>

std::string FileSize(std::string filename) {
	return System::Text::DeleteUnnecessaryZeros(std::to_string(System::File::GetFileSize(filename)));
}
void SetFilenames(std::string &filename, std::string &OutputAsm, std::string &OutputExe, char *argv[]) {
	filename = System::cpp::ArgumentManager<1>::get(argv);
	OutputAsm = System::File::WithoutExtention(filename) + ".asm";
	OutputExe = System::File::WithoutExtention(filename) + ".exe";
}
void DisplayCompilationInformation(size_t NbrOfToken, std::string filename, std::string OutputAsm, std::string OutputExe, double time, bool IsSucces) {
	std::cout << std::endl;
	if (NbrOfToken == 1) NbrOfToken = 0;
	LogMessage::LogMessage("Number of token processed: " + std::to_string(NbrOfToken), false);
	LogMessage::LogMessage("Original file size: " + FileSize(filename) + " bytes", false);
	if (IsSucces) {
		LogMessage::LogMessage("Compilation success!");
		LogMessage::LogMessage("Asm output file size: " + FileSize(OutputAsm), false);
		LogMessage::LogMessage("Exe output file size: " + FileSize(OutputExe), false);
	}
	else LogMessage::LogMessage("Compilation non success.");
	std::cout << std::endl << "Time taken: " <<
		System::Text::DeleteUnnecessaryZeros(std::to_string((clock() - time) / CLOCKS_PER_SEC)) << "s." << std::endl;
}

int main(int argc, char *argv[]) {
	System::Display::StartProgram("Arlia compiler", "vanaur", 0.01, "The compiler of Arlia programming language");

	clock_t tStart = clock();

	std::string filename, OutputAsm, OutputExe;
	SetFilenames(filename, OutputAsm, OutputExe, argv);

	std::remove(System::Text::StringToCharArray(LogMessage::LogFile));
	std::remove(System::Text::StringToCharArray(OutputExe));

	if (filename.empty()) {
		LogMessage::LogMessage("Can't continue compilation.");
		System::Display::ExitProgram(85);
		return 0;
	}

	size_t NbrOfToken = 0;
	bool StopAferFirstError = false;
	Exception exception;
	std::vector<std::string> HeaderImported;

	Lexer lexer(filename, StopAferFirstError);
	//Parser::parser parser;

	//parser.parse(lexer, NbrOfToken, HeaderImported, exception);
	//lexer.end();

	Assembler::AsmFinalCode code;
	code.IsDLL = true;

	/*
		/!\ -> Expressions matématique : fonctions intégrées dans les expressions !!
	*/

	Expr expr = lexer.GetLine();

	Parser::Function function;

	/*if (function.IsFunctionDeclaration(expr))
		if (function.IsAcceptableFunctionDeclaration(expr))
			function.AddFunction(function.ExprToFuncDecl(expr));*/

	tast::Ast ast;
	ast.append(expr);

	System::File::write(OutputAsm, code.GetAsm());

	// call FASM compiler here

	bool IsSucess = System::File::exist(OutputExe);

	DisplayCompilationInformation(NbrOfToken, filename, OutputAsm, OutputExe, tStart, IsSucess);

	System::Display::ExitProgram(85);
	return 0;
}
