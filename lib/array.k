object <type : typename> array {
	private:
		var container[] : type;
		ret index : integer = (sizeof(container) / sizeof(container[0]));
	public:
		ret size : integer = index;
		ret first : type = container[0];
		ret last : type = container[index];
		ret GetType : typename = type;
		func append(ToAppend : type) {
			container[index + 1] = ToAppend;
		}
		func at(i : integer) : type {
			if (i > index || i < 0) throw "Invliad index: " + i + ".";
			return container[i];
		}
		func insert(ToInsert : type, at : integer) {
			if (index == 0) throw "Can't insert an item into an empty array.";
			if (at > index) throw "Array stack over flow: acess to " + at + " index that not exist.";
			container[index + 1] = container[0]; `Just temporarily`
			for (i : integer = at to index) 
				container[i + 1] = container[i];
			container[at] = ToInsert;
		}
		func clear() {
			for (i : integer = 0 to index)
				delete &container[i];
		}
		func find(ToFind : type) : integer {
			for (i : integer = 0 to index)
				if (container[i] == ToFind) return i;
			throw "The value '" + ToFind + "' doesn't exist.";
		}
	upon:
		it --> container;
}
