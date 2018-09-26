#pragma once
#include "Parser.hpp"
#include "node.hpp"
#include "LL_NameConventions.hpp"

namespace Parser {
	class Identifier : public Leaf {
	private:
		std::string identifier_;
		void DoAccept(Visitor &v);
	public:
		
	};
}
