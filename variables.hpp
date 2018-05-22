#pragma once
#include "System.hpp"

/*
	[This namespace must remain accessible throughout the compilation]
	Each variable name must be unique in their block.
	If two variables defined in different blocks have the same name, there will be no errors.
	Careful:
		{
			var i = 9;
			{
				var i = 0;
			}
		}
	// There is a mistake here.
*/

namespace variable {
	class List {
	private:
		struct variable {
			std::string identifier;
			std::string type;
			size_t size;
			std::string value;
		};
		std::vector<variable> list;
	public:
		bool IsAlreadyExists(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier) return true;
			return false;
		}
		void Append(std::string identifier, std::string type, size_t size, std::string value) {
			list.push_back({ identifier, type, size, value });
		}
		std::string GetValueOf(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier)
					return var.value;
		}
		size_t GetSizeOf(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier)
					return var.size;
		}
		std::string GetTypeOf(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier)
					return var.type;
		}
		size_t size() {
			return list.size();
		}
	};
}
