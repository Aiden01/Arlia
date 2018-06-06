#pragma once
#include "System.hpp"
#include "functions.hpp"
#include "variables.hpp"
#include "AssemblerInsert.hpp"

/*
"object" is a procesus from APIL.
: proc "object" --> data structure;
*/

namespace objects {
	class List {
	private:
		struct object {
			std::string identifier;
			size_t size; // typesize = sum of fields; or typesize is defined by meta-programming.
			std::vector<object> inheritances;
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
		void Append(std::string identifier, size_t size, std::vector<functions::List> methods = {}, std::vector<variables::List> field = {}, std::vector<object> inheritances = {}) {
			list.push_back({ identifier, size, inheritances, methods, field });
		}
		std::string GetField(std::string obj_identifier, std::string field_identifier, int line = -1) {
			for (object obj : list)
				if (obj.identifier == obj_identifier)
					if (obj.identifier.empty())
						LogMessage::ErrorMessage("The indicated field '" + field_identifier + "' is not part of the '" + obj_identifier + "' object", line);
					else
						for (variables::List field : obj.fields)
							if (!field.IsAlreadyExists(field_identifier))
								LogMessage::ErrorMessage("The indicated field '" + field_identifier + "' is not part of the '" + obj_identifier + "' object", line);
							else
								if (field.IsVisible(field_identifier))
									return field.GetValueOf(field_identifier);
								else
									LogMessage::ErrorMessage("You do not have access to this field: '" + field.GetAbstractName(field_identifier) + "' in the '" + obj.identifier + "' object", line);
			return "";
		}
		std::string GetMethod(std::string obj_identifier, std::string method_identifier, std::multimap<int, std::string> args = {}, int line = -1) {
			for (object obj : list)
				if (obj.identifier == obj_identifier)
					if (obj.methods.empty())
						LogMessage::ErrorMessage("The indicated field '" + method_identifier + "' is not part of the '" + obj_identifier + "' object", line);
					else
						for (functions::List method : obj.methods)
							if (!method.IsAlreadyExists(method_identifier))
								LogMessage::ErrorMessage("The indicated field '" + method_identifier + "' is not part of the '" + obj_identifier + "' object", line);
							else
								if (method.IsVisible(method_identifier))
									return AssemblerInsert::CallFunction(method_identifier, args);
								else
									LogMessage::ErrorMessage("You do not have access to this field: '" + method.GetAbstractName(method_identifier) + "' in the '" + obj.identifier + "' object", line);
			return "";
		}
		std::string GetAbstractName(std::string identifier) {
			return "abstract";
		}
		size_t size(std::string identifier, int line = -1) {
			if (!IsAlreadyExists(identifier)) {
				LogMessage::ErrorMessage("This object does not exist: '" + GetAbstractName(identifier) + "'", line);
				return 0;
			}
			for (object obj : list)
				if (obj.identifier == identifier)
					return obj.size;
		}
		object get(std::string obj_identifier, int line = -1) {
			if (!IsAlreadyExists(obj_identifier))
				LogMessage::ErrorMessage("This object: '" + obj_identifier + "' doesn't exist", line);
			else
				for (object obj : list)
					if (obj.identifier == obj_identifier)
						return obj;
			return {"", 0, {}, {}, {}};
		}

		const object &operator[] (std::string obj_identifier) {
			return get(obj_identifier);
		}
	};
}
