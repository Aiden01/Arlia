#pragma once
#include "System.hpp"

namespace functions {
	class List {
	private:
		struct function {
			std::string identifier;
			std::string type;
			std::multimap<int, std::string> parameters; /// + adresses / optional / values
			bool visibility; // on-off in data structure / object
		};
		std::vector<function> list;
	public:
		bool IsAlreadyExists(std::string identifier) {
			for (function func : list)
				if (func.identifier == identifier) return true;
			return false;
		}
		void Append(std::string identifier, std::string type, std::multimap<int, std::string> args, bool visibility = true) {
			list.push_back({ identifier, type, args, visibility });
		}
		bool IsVisible(std::string identifier) {
			for (function func : list)
				if (func.identifier == identifier)
					return func.visibility;
		}
		std::string GetAbstractName(std::string identifier) {
			return "abstract";
		}
	};
}
