object <keyT : typename, valueT : typename> multimap {
	private:
		var KeyContainer[] : keyT;
		var ValueContainer[] : valueT;
		ret index : integer = (sizeof(KeyContainer) / sizeof(KeyContainer[0]));
	public:
		ret size : integer = index;
		ret GetTypeOfKey : typename = keyT;
		ret GetTypeOfValue : typename = valueT;
		func FindKey(ToFind : keyT) : integer {
			for (i : integer = 0 to index)
				if (KeyContainer[i] == ToFind) return i;
			throw "The key '" + ToFind + "' doesn't exist.";
		}
		func FindValue(ToFind : valueT) : integer {
			for (i : integer = 0 to index)
				if (ValueContainer[i] == ToFind) return i;
			throw "The value '" + ToFind + "' doesn't exist.";
		}
		func append(key : keyT, value : valueT) {
			KeyContainer[index + 1] = key;
			ValueContainer[index + 1] = value;
		}
		func clear() {
			for (i : integer = 0 to index) {
				delete &KeyContainer[i];
				delete &ValueContainer[i];
			}
		}
		func get(key : keyT) : valueT {
			return ValueContainer[FindKey(key)];
		}
	upon:
		` Syntax to review `
}
