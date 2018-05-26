#pragma once
#include "System.hpp"
#include "functions.hpp"
#include "variables.hpp"

/*
"object" is a procesus from APIL.
: proc "object" --> data structure;
*/

namespace objects {
	// Represents the operators that can affect the object (it).
	class uponit {
	private:
		struct upon {
			/// En pause pour le moment
		};
	public:

	};

	class List {
	private:
		struct object {
			std::string identifier;
			size_t size; // typesize = sum of fields; or typesize is defined by meta-programming.
			std::vector<List> inheritances;
			std::vector<functions::List> methods;
			std::vector<variables::List> fields;
			// std::vector<uponit> upons;
		};
		std::vector<object> list;
	public:
		bool IsAlreadyExists(std::string identifier) {
			for (object obj : list)
				if (obj.identifier == identifier) return true;
			return false;
		}
		void Append(std::string identifier, size_t size, std::vector<List> inheritances, std::vector<functions::List> methods, std::vector<variables::List> field) {
			list.push_back({ identifier, size, inheritances, methods, field });
		}
		std::string GetField(std::string obj_identifier, std::string field_identifier, int line = -1) {
			for (object obj : list)
				if (obj.identifier == obj_identifier)
					for (variables::List field : obj.fields)
						if (!field.IsAlreadyExists(field_identifier))
							System::Display::ErrorMessage("The indicated field '" + field_identifier + "' is not part of the '" + obj_identifier + "' object", line);
						else
							if (field.IsVisible(field_identifier))
								return field.GetValueOf(field_identifier);
							else
								System::Display::ErrorMessage("You do not have access to this field: '" + field.GetAbstractName(field_identifier) + "' in the '" + obj.identifier + "' object", line);
		end:
			return "";
		}
		std::string GetMethod(std::string obj_identifier, std::string method_identifier, int line = -1) {
			for (object obj : list)
				if (obj.identifier == obj_identifier)
					for (functions::List method : obj.methods)
						if (!method.IsAlreadyExists(method_identifier))
							System::Display::ErrorMessage("The indicated field '" + method_identifier + "' is not part of the '" + obj_identifier + "' object", line);
						else
						if (method.IsVisible(method_identifier))
							return "call mathod";
						else
							System::Display::ErrorMessage("You do not have access to this field: '" + method.GetAbstractName(method_identifier) + "' in the '" + obj.identifier + "' object", line);
			return "";
		}
		size_t size(std::string identifier) {
			for (object func : list)
				if (func.identifier == identifier)
					return func.size;
		}
	};
}
