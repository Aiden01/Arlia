#pragma once
#include "System.hpp"
#include "Assembler.hpp"

namespace Assembler {
	namespace Macro {
		
		const std::vector<std::string> Macros =
		{
			// Modulo
			"; Modulo macro\n"
			"macro mod a, b {\n"
			"\tif ~ eax eq a\n"
			"\t\tmov eax, a\n"
			"\tend if\n"
			"\tcdq\n"
			"\tdiv b\n"
			"\tif ~ edx eq a\n"
			"\t\tmov a, edx\n"
			"\tend if\n"
			"}\n"
		};

		inline std::string UseModulo(std::string a, std::string b) {
			return
				" ; Modulo procedure\n"
				" mov ebx, " + a + "\n"
				" mov ecx, " + b + "\n"
				" mod ebx, ecx\n";
		}
		inline void AppendMacros(AsmFinalCode &code) {
			for (std::string macro : Macros)
				code += macro;
		}
	}
}
