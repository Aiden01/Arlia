#include "compile.hpp"

int main(int argc, char *argv[]) {
	System::Display::StartProgram("Arlia compiler", "vanaur", 0.01, "The compiler of Arlia programming language");

	std::string filename = System::cpp::ArgumentManager<1>::get(argv);

	Compile compiler(filename);
	compiler.run();
	compiler.write();
	compiler.DisplayCompilationInformation();

	System::Display::ExitProgram(85);
	return 0;
}
