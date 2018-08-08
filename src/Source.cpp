#include "System.hpp"
#include "LogMessage.hpp"
#include "Lexer.hpp"
#include "Errors.hpp"
#include "Parser.hpp"
#include "LL_NameConventions.hpp"
#include "MathExpression.hpp"
#include "AssemblerMacros.hpp"

std::string FileSize(std::string filename) {
	return System::Text::DeleteUnnecessaryZeros(std::to_string(System::File::GetFileSize(filename)));
}

void DisplayCompilationInformation(size_t NbrOfToken, std::string filename, std::string OutputAsm, std::string OutputExe, bool IsSucces) {
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
}

int main(int argc, char *argv[]) {
	System::Display::StartProgram("Arlia compiler", "vanaur", 0.01, "The compiler of Arlia programming language");

	std::string filename = System::cpp::ArgumentManager<1>::get(argv);
	std::string OutputAsm = System::File::WithoutExtention(filename) + ".asm";
	std::string OutputExe = System::File::WithoutExtention(filename) + ".exe";

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

	Math::Expression math;
	
	std::vector<token_t> expr = lexer.GetLine();
	if (math.IsMathExpression(expr))
		if (math.CanInterpret(expr))
			std::cout << math.Interpret(expr) << std::endl;
		else
			std::cout << math.ConvertToAsm(expr) << std::endl;
	Assembler::Register reg;
	std::cout << reg.MovInStack(4, "[edi]") << std::endl;
	

	System::File::write(OutputAsm, code.GetAsm());

	// call FASM compiler here

	bool IsSucess = System::File::exist(OutputExe);

	DisplayCompilationInformation(NbrOfToken, filename, OutputAsm, OutputExe, false);

	System::Display::ExitProgram(85);
	return 0;
}
