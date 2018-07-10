#pragma once
#include "System.hpp"
#include "array.hpp"

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

namespace variables {
	class List {
	public:
		static struct variable {
			std::string identifier;
			std::string adress;		// ex: [esp-4]
			std::string type;
			size_t size;
			std::string value;
			bool visibility; // on-off in data structure / object
			bool IsConst;
		};
	private:
		std::vector<variable> list;
	public:
		std::string CurrentFile{ "" };
		bool IsAlreadyExists(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier) return true;
			return false;
		}
		void Append(std::string identifier, std::string adress, std::string type, size_t size, std::string value = "", bool visibility = true, bool IsConst = false) {
			list.push_back({ identifier, adress, type, size, value, visibility, IsConst });
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
		bool IsVisible(std::string identifier) {
			for (variable var : list)
				if (var.identifier == identifier)
					return var.visibility;
		}
		std::string GetAbstractName(std::string identifier) {
			return "abstract";
		}
		size_t size(std::string identifier, int line = -1) {
			if (!IsAlreadyExists(identifier)) {
				LogMessage::ErrorMessage("This variable does not exist: '" + GetAbstractName(identifier) + "'", CurrentFile, line);
				return 0;
			}
			for (variable var : list)
				if (var.identifier == identifier)
					return var.size;
		}
		variable get(std::string var_identifier, int line = -1) {
			if (!IsAlreadyExists(var_identifier))
				LogMessage::ErrorMessage("This variable: '" + var_identifier + "' doesn't exist", CurrentFile, line);
			else
				for (variable var : list)
					if (var.identifier == var_identifier)
						return var;
			return { "", "", "", 0, "", false, false };
		}

		const variable &operator[] (std::string var_identifier) {
			return get(var_identifier);
		}
	};
}

