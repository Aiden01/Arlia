#pragma once
#include "System.hpp"
#include "AssemblerInsert.hpp"
#include "functions.hpp"

/*
	This header file handles potential exceptions caused by the user, code, or program.
	It contains a list of error messages directly embedded in the target file.
	If a test is not approved during the execution, and no try-catch / throw block exists,
	the program redirects itself to one of these automatic error messages, and launches a dialog box.
*/

namespace ExceptionHandling {
	// https://www.freepascal.org/docs-html/user/usersu100.html
	enum exceptions {
		MallocFailure,
		DivideByZero,
		StackOverflow,
		StackUnderflow,
		OutOfRange
	};
	std::multimap<std::string, std::string> Untried_msg = {
		{ "_excpt_MallocFailure", "Dynamic allocation error" },
	{ "_excpt_DivideByZero", "Divide by 0" },
	{ "_excpt_StackOverflow", "Stack overflow" },
	{ "_excpt_StackUnderflow", "Stack underflow" },
	{ "_excpt_OutOfRange", "Value out of range" }
	};
	void AddToCode(AssemblerInsert::FinalCode *code, functions::List *FuncList) {
		*code += AssemblerInsert::Library::use("MessageBoxA", "user32", "INCLUDE\\win32a.inc");
		*code += "; Basic error message in case of exception detected by the program itself during execution:\n";
		typedef void (AssemblerInsert::FinalCode::*AppendGlobalVariable)(int, std::string);
		typedef void (functions::List::*Append)(std::string, std::string, std::map<int, std::string>);
		for (std::map<std::string, std::string>::iterator it = Untried_msg.begin(); it != Untried_msg.end(); ++it) {
			std::string msg = "[Error] Exception generated : { " + it->second + " }";
			(code->AppendGlobalVariable)(sizeof(char), "\"" + msg + "\"");
			*code += it->first + ":\n";
			*code += "\tinvoke MessageBoxA, 0, GVUN" + std::to_string(AssemblerInsert::GVUN_n - 1) + ", 0, MB_ICONERROR+MB_RETRYCANCEL\n";
			*code += "\tcmp eax, IDRETRY\n";
			*code += "\tjne _StopProgram\n";
			*code += "\tret\n";
			(FuncList->Append)(it->first, "void", 0, { {} });
		}
	}
	void GoTo(exceptions exception, AssemblerInsert::FinalCode *code) {
		switch (exception) 		{
		case ExceptionHandling::MallocFailure:
			*code += "\tcall _excpt_MallocFailure\n";
			break;
		case ExceptionHandling::DivideByZero:
			*code += "\tcall _excpt_DivideByZero\n";
			break;
		case ExceptionHandling::StackOverflow:
			*code += "\tcall _excpt_StackOverflow\n";
			break;
		case ExceptionHandling::StackUnderflow:
			*code += "\tcall _excpt_StackUnderflow\n";
			break;
		case ExceptionHandling::OutOfRange:
			*code += "\tcall _excpt_OutOfRange\n";
			break;
		}
	}
}
