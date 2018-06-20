#include "System.hpp"

namespace Array {
	class List {
	public:
		struct Array {
			std::string identifier;
			std::string adress;
			std::string type;
			size_t size;
			size_t cbits; // size of each item
			std::vector<std::string> value;
			bool visibility;
			bool IsConst;
		};
	private:
		std::vector<Array> list;
	public:
		bool IsAlreadyExists(std::string identifer) {
			for (Array tab : list)
				if (tab.identifier == identifer) return true;
			return false;
		}
		std::string GetAbstractName(std::string identifier) {
			return "abstract";
		}
		size_t size(std::string identifier, int line = -1) {
			if (!IsAlreadyExists(identifier)) {
				LogMessage::ErrorMessage("This variable does not exist: '" + GetAbstractName(identifier) + "'", line);
				return 0;
			}
			for (Array tab : list)
				if (tab.identifier == identifier)
					return tab.size;
		}
		std::string adress(std::string identifier, int line = -1) {
			if (!IsAlreadyExists(identifier)) {
				LogMessage::ErrorMessage("This variable does not exist: '" + GetAbstractName(identifier) + "'", line);
				return 0;
			}
			for (Array tab : list)
				if (tab.identifier == identifier)
					return tab.adress;
		}
		void Append(std::string identifier, std::string adress, std::string type, size_t size, size_t cbits, std::vector<std::string> value = { "" }, bool visibility = true, bool IsConst = false) {
			list.push_back({ identifier, adress, type, size, cbits, value, visibility, IsConst });
		}
		Array get(std::string identifier, int line = -1) {
			if (!IsAlreadyExists(identifier))
				LogMessage::ErrorMessage("This variable: '" + identifier + "' doesn't exist", line);
			else
				for (Array tab : list)
					if (tab.identifier == identifier)
						return tab;
			return { "", "", "", 0, 0,{ "" }, false, false };
		}
		std::string GetValueAt(std::string identifier, int index, int line = -1) {
			if (!IsAlreadyExists(identifier))
				LogMessage::ErrorMessage("This variable: '" + identifier + "' doesn't exist", line);
			else {
				for (Array tab : list) {
					if (tab.identifier == identifier) {
						if (tab.value.size() - 1 < index) {
							LogMessage::ErrorMessage("There are no values defined in index [" + std::to_string(index) + "] of '" + tab.identifier + "' array.", line);
							return "";
						}
						else return tab.value[index];
					}
				}
			}
		}
		std::string GetAdressAt(std::string identifier, int index, int line = -1) {
			if (!IsAlreadyExists(identifier))
				LogMessage::ErrorMessage("This variable: '" + identifier + "' doesn't exist", line);
			else
				for (Array tab : list)
					if (tab.identifier == identifier)
						return tab.adress + "-" + std::to_string(index * tab.cbits);
		}
		void DestroyArray(std::string identifier) {

		}
	};
}
