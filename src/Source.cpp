#include "System.hpp"
#include "CompilerLog_msg.hpp"
#include "AssemblerInsert.hpp"
#include "tokenizer.hpp"
#include "identifier.hpp"
#include "oop.hpp"
#include "statements.h"
#include "preprocessor.hpp"

int main(int argc, char *argv[]) {
	System::Display::StartProgram("Funnel", "vnr", 00.01, "Arlia's compiler");
	std::string RawCode = System::File::GetAllText(System::cpp::ArgumentManager<1>::get(argv));
	if (RawCode.empty()) goto end;
	{
		std::string output = System::File::WithoutExtention(System::Text::CharArrayToString(argv[1])) + ".asm";
		System::File::write(output, "");
		/* ------- */
		tokenizer::tokenize tokens(RawCode);
		/* ------- */
		AssemblerInsert::FinalCode code;
		functions::List FuncList;
		variables::List VarList;
		objects::List ObjList;
		Preprocessor::Defines defines;
		/* ------- */

		for (std::string token : tokens.GetTokens())
			std::cout << token << std::endl;

		/* ------- */
		System::File::WriteAppend(output, code.get());
	}
end:
	System::Display::ExitProgram();
	return 0;
}
