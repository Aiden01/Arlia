#pragma once
#include "System.hpp"
#include "AssemblerInsert.hpp"

/*
	This header file handles potential exceptions caused by the user, code, or program.
	It contains a list of error messages directly embedded in the target file.
	If a test is not approved during the execution, and no try-catch / throw block exists,
	the program redirects itself to one of these automatic error messages, and launches a dialog box.
*/

namespace ExceptionHandling {
	enum exceptions {
		MallocFailure
	};
	std::multimap<std::string, std::string> Untried_msg = {
		{ "_excpt_MallocFailure", "Dynamic allocation error" }
	};
	void AddToCode(AssemblerInsert::FinalCode *code) {
		*code += AssemblerInsert::Library::use("puts", "msvcrt", "INCLUDE\\win32a.inc");
		for (std::map<std::string, std::string>::iterator it = Untried_msg.begin(); it != Untried_msg.end(); ++it) {
			*code += it->first + ":\n";
			*code += AssemblerInsert::Library::invoke("puts", { {sizeof(char), "\"{" + it->second + "}\""} }, code);
			*code += "\tret\n";
		}
	}
	void GoTo(exceptions exception, AssemblerInsert::FinalCode *code) {
		// j'ai là.
	}
}
