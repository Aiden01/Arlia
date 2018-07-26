#include "System.hpp"
#include "LogMessage.hpp"
#include "Lexer.hpp"

int main(int argc, char *argv[]) {
	System::cpp::clear();

	Lexer lexer(System::cpp::ArgumentManager<1>::get(argv));
	if (lexer.CanContinue == false) goto end;

	while (!lexer.eof) {
		std::cout << lexer.next().value << std::endl;
	}

end:
	return 0;
}
