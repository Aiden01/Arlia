#include "System.hpp"
#include "ExceptionHandling.hpp"
#include "CompilerLog_msg.hpp"
#include "AssemblerInsert.hpp"
#include "tokenizer.hpp"
#include "identifier.hpp"
#include "oop.hpp"
#include "statements.h"
#include "preprocessor.hpp"
#include "lexer.hpp"
#include "Parser.hpp"
#include "ArliaBase.hpp"

#include <time.h>

void EndInfos(std::string filename, bool exe = true) {
	LogMessage::LogMessage("End of compilation");
	if (exe) LogMessage::LogMessage("Generated files: \n  - " + filename + ".asm\n  - " + filename + ".exe");
	else LogMessage::LogMessage("Generated file: \n  - " + filename + ".asm");
	LogMessage::LogMessage("Asm file size: " + System::Text::DeleteUnnecessaryZeros(std::to_string(System::File::GetFileSize(filename + ".asm"))) + " bytes");
	if (exe) LogMessage::LogMessage("Exe file size: " + System::Text::DeleteUnnecessaryZeros(std::to_string(System::File::GetFileSize(filename + ".exe"))) + " bytes");
}
bool CanCompile(int NumberOfErrors) {
	if (NumberOfErrors == 0) return true;
	LogMessage::LogMessage("There are too many errors to compile: " + std::to_string(NumberOfErrors) + " error(s)");
	return false;
}

int main(int argc, char *argv[]) {
	clock_t tStart = clock();
	System::Display::StartProgram("Funnel", "vnr", 00.01, "Arlia's compiler");
	std::string RawCode = System::File::GetAllText(System::cpp::ArgumentManager<1>::get(argv));
	std::string filename = System::cpp::ArgumentManager<1>::get(argv, false);
	std::string output = System::File::WithoutExtention(System::Text::CharArrayToString(argv[1])) + ".asm";
	if (RawCode.empty()) goto end;
	{
		remove("ARLOG.log");
		remove(System::Text::StringToCharArray(System::File::WithoutExtention(filename) + ".exe"));
		System::File::write(output, "");
		/* ----{ Tokenization / Lexing }---- */
		std::vector<std::string> tokens = tokenizer::GetTokens(RawCode, filename);
		if (tokens.empty()) goto end;
		Lexer lexer(tokens);
		Parser parser(lexer.get());
		/* ----{ Variables / pre-processor / functions / objects management / definition }---- */
		/// toutes ces déclarations devraient être dans Parser, ainsi que Assembler::Setentrypoint
		AssemblerInsert::FinalCode code;
		AssemblerInsert::Heap stack(&code);
		functions::List FuncList;
		variables::List VarList;
		Array::List VarArrayList;
		objects::List ObjList;
		Preprocessor::Defines defines;
		bool main = false;
		/* ----{ Parsing / test zone }---- */
		AssemblerInsert::SetEntryPoint(&code, &stack, &FuncList, &main);



		/* ----{ Reserved functions }---- */
		AssemblerInsert::_StopProgram(&code);
		ExceptionHandling::AddToCode(&code, &FuncList);
		/* ----{ Writing and compiling assembler code }---- */
		if (!main) {
			LogMessage::ErrorMessage("The entry point is not defined");
			goto end;
		}
		if (!CanCompile(LogMessage::ErrorCounter)) goto end;
		System::File::WriteAppend(output, code.get());
	}
	System::Display::ExitProgram();
	remove(System::Text::StringToCharArray(System::File::WithoutExtention(filename) + ".exe"));
	system(System::Text::StringToCharArray("FASM.exe " + output));
end:
	System::Display::ExitProgram();
	EndInfos(System::File::WithoutExtention(filename), System::File::exist(System::File::WithoutExtention(filename) + ".exe"));
	std::cout << std::endl << "Time taken: " << ((float)(clock() - tStart) / CLOCKS_PER_SEC) << "s" << std::endl;
	System::Display::ExitProgram();
	return 0;
}
