#pragma once
#include "Maker.hpp"

namespace Parser {
	class Types {
		enum Primitives { Int, Bool, Float, Real, String, Char };
	private:
		struct Type {
			std::string identifier;
			size_t size;
			/*
				- fields
				- methods
				- constructor
				- image
				- template
			*/
		};

		std::vector<Type> types;

	public:

	};
}
