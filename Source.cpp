#include "System.hpp"
#include "LogMessage.hpp"
#include "Lexer.hpp"

int main(int argc, char *argv[]) {
	System::cpp::clear();
	Lexer lexer("hello.arl");

	if (lexer.CanContinue) {
		while (!lexer.eof || lexer.CanContinue) {
			std::cout << lexer.next().value << std::endl;
		}
	}

	return 0;
}
