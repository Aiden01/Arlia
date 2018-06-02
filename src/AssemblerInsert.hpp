#pragma once
#include "System.hpp"

/*
This file contains functions to facilitate abstract level crossings.
*/

namespace AssemblerInsert {

	const std::string Stack = "ebp";
	const std::string StackPointer = "sp";
	const std::string StackFrame = "esp";

	class Register {
	private:
		int counter_8 = 0;
		int counter_16 = 0;
		int counter_32 = 0;
		int counter_64 = 0;
		int counter_80 = 0;
		int counter_128 = 0;
		int counter_256 = 0;
		int counter_512 = 0;

		int StackSize = 0;
		std::stack<int> StackSizes; // Contains all size of values in the stack

		const int max = 8;
	private:
		std::vector<std::string> bits_8 = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
		std::vector<std::string> bits_16 = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };
		std::vector<std::string> bits_32 = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
		std::vector<std::string> bits_64 = { "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7" };
		std::vector<std::string> bits_80 = { "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7" };
		std::vector<std::string> bits_128 = { "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7" };
		std::vector<std::string> bits_256 = { "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7" };
		std::vector<std::string> bits_512 = { "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7" };
	public:
		void reset(int size) {
			if ((size * 8) % 8 == 0) counter_8 = 0;
			if ((size * 8) % 16 == 0) counter_16 = 0;
			if ((size * 8) % 32 == 0) counter_32 = 0;
			if ((size * 8) % 64 == 0) counter_64 = 0;
			if ((size * 8) % 80 == 0) counter_80 = 0;
			if ((size * 8) % 128 == 0) counter_128 = 0;
			if ((size * 8) % 256 == 0) counter_256 = 0;
			if ((size * 8) % 512 == 0) counter_512 = 0;
		}
		std::string next(int size, std::vector<std::string> separate = { StackFrame, Stack, Stack }) {
			std::string ret = "reg";
			if ((size * 8) % 8 == 0) {
				if (counter_8 == max) reset(size);
				ret = bits_8[counter_8];
				++counter_8;
			}
			if ((size * 8) % 16 == 0) {
				if (counter_16 == max) reset(size);
				ret = bits_16[counter_16];
				++counter_16;
			}
			if ((size * 8) % 32 == 0) {
				if (counter_32 == max) reset(size);
				ret = bits_32[counter_32];
				++counter_32;
			}
			if ((size * 8) % 64 == 0) {
				if (counter_64 == max) reset(size);
				ret = bits_64[counter_64];
				++counter_64;
			}
			if ((size * 8) % 80 == 0) {
				if (counter_80 == max) reset(size);
				ret = bits_80[counter_80];
				++counter_80;
			}
			if ((size * 8) % 128 == 0) {
				if (counter_128 == max) reset(size);
				ret = bits_128[counter_128];
				++counter_128;
			}
			if ((size * 8) % 256 == 0) {
				if (counter_256 == max) reset(size);
				ret = bits_256[counter_256];
				++counter_256;
			}
			if ((size * 8) % 512 == 0) {
				if (counter_512 == max) reset(size);
				ret = bits_512[counter_512];
				++counter_512;
			}
			
			// Prevents the use of stack registers in program manipulation
			if (System::Vector::Contains<std::string>(separate, ret)) return next(size, separate);

			return ret;
		}
		void ReverseOrder(int size = -1) {
			if (size == -1) for (int i = 1; i < 8; ++i) ReverseOrder(i * 8);
			if ((size * 8) % 8 == 0) std::reverse(bits_8.begin(), bits_8.end());
			if ((size * 8) % 16 == 0) std::reverse(bits_16.begin(), bits_16.end());
			if ((size * 8) % 32 == 0) std::reverse(bits_32.begin(), bits_32.end());
			if ((size * 8) % 64 == 0) std::reverse(bits_64.begin(), bits_64.end());
			if ((size * 8) % 80 == 0) std::reverse(bits_80.begin(), bits_80.end());
			if ((size * 8) % 128 == 0) std::reverse(bits_128.begin(), bits_128.end());
			if ((size * 8) % 256 == 0) std::reverse(bits_256.begin(), bits_256.end());
			if ((size * 8) % 512 == 0) std::reverse(bits_512.begin(), bits_512.end());
		}
		std::string	PushInStack(int size) {
			StackSize += (size * 2);
			StackSizes.push(StackSize / 2);
			return "[" + Stack + "-" + std::to_string(StackSizes.top()) + "]";
		}
	};

	class Library {
	public:
		// example: use 'ExitProcess', from 'kernel32', in 'INCLUDE\\win32a.inc'
		static std::string use(std::string name, std::string from, std::string in) {
			return "use '" + name + "', from '" + from + "', in '" + in + "'" + "\n";
		}
		static std::string invoke(std::string name, std::vector<std::string> args) {
			std::string ret;
			ret += "\tinvoke " + name;
			for (std::string arg : args)
				ret += ", " + arg;
			return ret + "\n";
		}
	};

	class FinalCode {
	private:
		std::vector<std::string> libraries;
		std::vector<std::string> includes;
		std::map<std::string, std::vector<std::string>> imports; // [0] -> lib [1] -> names
		std::string RawCode;

		// UFI --> Use From In

		void builUFI(std::string library, std::string import, std::string name) {
			bool lib = true;
			bool imp = true;
			std::string ret;

			if (System::Vector::Contains<std::string>(libraries, library)) lib = false;
			if (System::Vector::Contains<std::string>(includes, import)) imp = false;

			if (lib && imp) {
				libraries.push_back(library);
				includes.push_back(import);
			}
			else if (lib) libraries.push_back(library);
			else if (imp) includes.push_back(import);

			imports[library].push_back(name);

		}
		bool IsUFI(std::string toparse) {
			if (System::Text::contains(toparse, "use") &&
				System::Text::contains(toparse, "from") &&
				System::Text::contains(toparse, "in") && 
				System::Text::occurence(toparse, ",") == 2)
			return true;
			return false;
		}
		// Just add
		void ParseUFI(std::string toparse) {
			// example: use 'printf', from 'msvcrt', in 'INCLUDE\win32a.inc'

			std::string library;
			std::string import;
			std::string name;

			for (std::string str : System::Vector::split(System::Text::replace(toparse, " ", ""), ",")) {
				if (System::Text::StartsWih(str, "use")) name = str.substr(3, str.length());
				else if (System::Text::StartsWih(str, "from")) library = str.substr(4, str.length());
				else if (System::Text::StartsWih(str, "in")) import = str.substr(2, str.length());
			}

			builUFI(System::Text::replace(library, "'", ""), System::Text::replace(import, "'", ""), System::Text::replace(name, "'", ""));
		}

		const std::string section_IDATA = "section '.idata' data readable import ; idata\n";
		const std::string section_DATA = "section '.data' data readable writeable";
		const std::string section_CODE = "section '.code' code executable ; code\n";

		std::string outset() {
			std::string ret;
			if (System::cpp::GetOS() == System::cpp::Windows) ret += "format PE\n";
			else if (System::cpp::GetOS() == System::cpp::Unix) ret += "format ELF\n";
			ret += "entry main\n\n";
			return ret;
		}

	public:
		void append(std::string toappend, bool CRLF = false) {
			if (IsUFI(toappend)) {
				ParseUFI(toappend);
				return;
			}
			if (CRLF) RawCode += toappend + "\n";
			else RawCode += toappend;
		}
		std::string get() {
			// First, it is necessary to remove duplicates in order to avoid errors: 
			for (std::map<std::string, std::vector<std::string>>::iterator it = imports.begin(); it != imports.end(); ++it)
				System::Vector::RemoveDuplicates<std::string>(it->second);
			std::string ret;
			ret += outset();
			if (!libraries.empty() || !imports.empty())
				ret += section_IDATA;
			for (std::string inc : includes)
				ret += "include '" + System::Text::replace(inc, "\n", "") + "'" + "\n";
			if (!libraries.empty()) {
				ret += "library ";
				ret += libraries[0] + ", " + "'" + libraries[0] + ".dll" + "'";
				if (libraries.size() > 1) ret += ", \\";
				for (int i = 1; i < libraries.size(); ++i)
					if (i == libraries.size() - 1)
						ret += "\n\t" + libraries[i] + ", " + "'" + libraries[i] + ".dll" + "'";
					else
						ret += "\n\t" + libraries[i] + ", " + "'" + libraries[i] + ".dll" + "'" + ", \\";
				ret += "\n";
			}
			for (std::map<std::string, std::vector<std::string>>::iterator it = imports.begin(); it != imports.end(); ++it) {
				ret += "import " + it->first;
				for (std::string name : it->second)
					ret += ", " + name + ", " + "'" + name + "'";
				ret += "\n";
			}
			/// A modifier peux-être...
			ret += "\n" + section_CODE;
			///
			ret += RawCode;
			return ret;
		}
		FinalCode &operator+=(const std::string &other) {
			this->append(other);
			return *this;
		}
	};

	std::string _namespace(std::string NamespaceName) {
		return "_NMSPC" + NamespaceName;
	}
	std::string _structure(std::string StructureName) {
		return "_STRCTR" + StructureName;
	}
	std::string _function(std::string FunctionName) {
		return "_FNC" + FunctionName;
	}

	std::string EndLabel() {
		return "\tpop " + Stack + "\n\tret\n\n";
	}

	std::string OperatorIdentifier(int Bits) {
		switch (Bits * 2) {
		case 2: return "BYTE";
		case 8: return "DWORD";
		case 16: return "WORD";
		case 32: return "DWORD";
		case 48: return "FWORD";
		case 64: return "PWORD";
		case 80: return "QWORD";
		case 128: return "TWORD";
		case 256: return "QQWORD";
		case 512: return "DQQWORD";
		default: return "";
		}
	}

	// int -> size | string -> identifier
	std::string SetFunctionParameter(std::string FuncName, std::multimap<int, std::string> parameters) {
		std::string ret;
		ret += FuncName + ":\n";
		ret += "\tpush " + Stack + "\n";
		ret += "\tmov " + Stack + ", " + StackFrame + "\n";
		int alloc = 0;
		Register reg;
		reg.ReverseOrder();
		if (parameters.empty()) goto end;
		for (std::map<int, std::string>::iterator it = parameters.begin(); it != parameters.end(); ++it) ++alloc;
		for (std::map<int, std::string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
			ret += "\tmov " + OperatorIdentifier(it->first) + " [" + Stack + "-" + std::to_string((it->first * std::distance(parameters.begin(), it)) + it->first) + "], " + reg.next(it->first) + " ; " + it->second + " -> " + OperatorIdentifier(it->first) + " (" + std::to_string(it->first) + " bits" + ")" + "\n";
	end:
		return ret;
	}
	// int -> size | string -> value
	std::string CallFunction(std::string name, std::multimap<int, std::string> parameters) {
		int ParamNbr = 0;
		std::string ret;
		Register reg;
		reg.ReverseOrder();
		for (std::map<int, std::string>::iterator it = parameters.begin(); it != parameters.end(); ++it) ++ParamNbr;
		for (std::map<int, std::string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
			ret += "\tmov " + OperatorIdentifier(it->first) + " " + reg.next(it->first) + ", " + it->second + "\n";
		ret += "\tcall " + name + "\n";
		return ret;
	}

	std::string SetEntryPoint() {
		std::string ret =
			"main:\n"
			"\tpush " + Stack + "\n"
			"\tmov " + Stack + ", " + StackFrame + "\n"
			"\tmov DWORD [" + Stack + "-4], edi\n"
			"\tmov DWORD [" + Stack + "-16], esi\n";
		return ret;
	}
	std::string EndEntryPoint() {
		std::string ret;
		ret += "\tmov " + StackFrame + ", " + Stack + "\n";
		ret += "\tpop " + Stack + "\n";
		ret += "\tret\n";
		ret += "\n";
		return ret;
	}

	std::string SetGlobalVar(std::string name, int bits, std::string value = "?") {
		switch (bits) {
		case 0: return name + " db " + value + "\n";
		case 1: return name + " db " + value + "\n";
		case 2: return name + " dw " + value + "\n";
		case 4: return name + " dd " + value + "\n";
		case 6: return name + " dp " + value + "\n";
		case 8: return name + " dq " + value + "\n";
		case 10: return name + " dt " + value + "\n";
		default: return SetGlobalVar(name, (bits * bits) % 4, value);
		}
	}

	// dimension -> [][]...
	std::string SetArray(std::string name, int dimension, int bits, std::vector<std::string> values = { "" }, size_t size = 0) {
		if (values.size() > 0 && size == 0) size = values.size();
		std::string OpId = OperatorIdentifier(bits);
		std::string ret;
		Register reg;
		reg.ReverseOrder();
		for (int i = 0; i < size; ++i)
			ret += "mov " + OpId + " " + reg.PushInStack(bits) + ", " + values[i] + "\n";
		return ret;
	}

}
