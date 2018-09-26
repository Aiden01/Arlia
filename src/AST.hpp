#pragma once
#include "Parser.hpp"
#include "node.hpp"
#include "visitor.hpp"

namespace Parser {
	class AST {
	private:
		Node &_node;
	public:
		AST(Node &node) : _node{ node } {};
		Node &root() { return _node; }
		Node const &root() const { return _node; }
	};
}
