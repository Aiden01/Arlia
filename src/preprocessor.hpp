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
		void AppendDef(std::string name, std::string value = "") {
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
	class Import {
	public:
		std::vector<std::string> headers;
		bool IsGood(std::array<std::string, 3> ImportFileStatements) {
			if (ImportFileStatements[0] != "import") return false;
			if (!System::Text::IsString(ImportFileStatements[1])) return false;
			if (ImportFileStatements[2] != ";") return false;
			std::string file = ImportFileStatements[1].substr(1, ImportFileStatements[1].length() - 2);
			if (System::File::exist(file)) headers.push_back(file);
			else {
				LogMessage::ErrorMessage("The header file doesn't exist: '" + file + "'");
				return false;
			}
			return true;
		}
		std::vector<std::string> ImportAll() {
			std::vector<std::string> ret;
			for (std::string header : headers)
				ret.push_back(System::File::GetAllText(header));
			return ret;
		}
	};
}
