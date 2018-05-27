#include "System.hpp"
#include "CompilerLog_msg.hpp"
#include "AssemblerInsert.hpp"
#include "tokenizer.hpp"
#include "identifier.hpp"
#include "oop.hpp"

int main(int argc, char *argv[]) {
	System::Display::StartProgram("Funnel", "vnr", 00.01, "Arlia's compiler");
	std::string output = "FasmFileTest.asm";

	// std::string RawCode = System::File::GetAllText(System::cpp::ArgumentManager<1>::get(argv));

	System::File::write(output, "; Code generated by the Funnel compiler (c) of the Arlia language (c).\n; This is currently a pre-alpha version, the output still requires optimizations / retouching.\n; Assembler language used : FASM.\n\n");
	AssemblerInsert::FinalCode code;
	/* ------- */

	/*tokenizer::tokenize tokens(RawCode);
	for (std::string str : tokens.GetLines())
	std::cout << str << std::endl;*/

	// std::cout << AssemblerInsert::SetArray("MyArray", sizeof(int), { "65","87","23" }) << std::endl;

	objects::List obj;
	objects::List inheritances;
	functions::List method;
	variables::List field;

	field.size("foo");

	/* ------- */
	System::File::WriteAppend(output, code.get());
	System::Display::ExitProgram();
	return 0;
}
