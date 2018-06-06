#pragma once
#include "System.hpp"
#include "statements.h"

/*
preprocessor directives:
import
define
proc
*/

namespace Preprocessor {
	class Defines {
	private:
		std::map<std::string, std::string> defines;
	public:
		std::string GetValueOf(std::string name) {
			return System::Map::GetItem<std::string, std::string>(defines, name);
		}
		void AppendDef(std::string name, std::string value) {
			defines.insert(std::pair<std::string, std::string>(name, value));
		}
		bool IsAlreadyExist(std::string name) {
			return (System::Map::Contains(defines, name));
		}
	};
	class Procs {
	private:
		struct proc {
			
		};
	public:

	};
}
