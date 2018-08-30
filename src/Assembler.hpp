#pragma once
#include "System.hpp"
#include "LogMessage.hpp"

/*
	+-----------------------------------------------------------------------------+
	| Assembler.h/cpp makes the high-level to low-level transition easier         |
	| by offering methods that match the specific compilation and language needs. |
	+-----------------------------------------------------------------------------+
*/



namespace Assembler {

	const std::string Stack_reg = "ebp";
	const std::string StackPointer_reg = "sp";
	const std::string StackFrame_reg = "esp";

	class Include {
	public:
		std::vector<std::string> Libraries, Includes;
		std::map<std::string, std::vector<std::string>> Imports; // [0] -> lib [1] -> function names

		// UFI = Use From In

		bool IsUFI(std::string StrToParse);
		void AddUFI(std::string LibName, std::string ImportName, std::string IncName);

		static std::string use(std::string name, std::string from, std::string in);
	};

	class AsmFinalCode : public Include {
	private:
		const std::string section_IDATA = "section '.idata' data readable import\n";
		const std::string section_DATA = "section '.data' data readable writeable\n";
		const std::string section_CODE = "section '.code' code executable\n";

		size_t GlobalVariableUniqueNameIndex = 0; // Index of global variables in asm
		struct asm_GlobalVariable {
			std::string UniqueName, Bytes, Value;
		};

		std::vector<asm_GlobalVariable> GlobalVariables;

		std::string outset();
		void AppendGlobalVariable(int bytes, std::string value = "?");
		void PrepareCode();

		std::string RawCode;

	public:
		static std::string OperatorIdentifier(int bytes);
		bool IsDLL = false;
		void append(std::string ToAppend);
		std::string GetAsm();
		AsmFinalCode &operator+=(const std::string &ToAppend) {
			this->append(ToAppend);
			return *this;
		}
	};

	class Register {
	private:
		size_t counter_8 = 0, counter_16 = 0, counter_32 = 0, counter_64 = 0, 
			   counter_80 = 0, counter_128 = 0, counter_256 = 0, counter_512 = 0;
		size_t StackSize = 0;
		size_t const max = 8;
		std::stack<size_t> StackSizes; // Contains all size of values in the stack

		std::vector<std::string> bits_8 = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
		std::vector<std::string> bits_16 = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };
		std::vector<std::string> bits_32 = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
		std::vector<std::string> bits_64 = { "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7" };
		std::vector<std::string> bits_80 = { "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7" };
		std::vector<std::string> bits_128 = { "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7" };
		std::vector<std::string> bits_256 = { "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7" };
		std::vector<std::string> bits_512 = { "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7" };

	public:
		void reset(size_t size);
		void ReverseOrder(int size = -1);
		void ResetStack();
		std::string next(size_t size, std::vector<std::string> separate = { StackFrame_reg, Stack_reg });
		std::string	PushInStack(size_t size, bool subtract = true);
		std::string MovInStack(size_t size, std::string value, bool substract = true);
	};

	typedef void (AsmFinalCode::*AppendGlobalVariable)(int, std::string); // (code->AppendGlobalVariable)(first, second);

	

}

