#pragma once
#include "visitor.hpp"

namespace Parser {

	class Node {
	public:
		using NodePtr = std::unique_ptr<Node>;
		Node(const Node&) = delete;
		Node &operator=(const Node&) = delete;
		Node() = default;
		virtual ~Node() = default;
		virtual void analyze(Visitor&) = 0;
		virtual Node &InsertLeft(NodePtr&&) = 0;
		virtual Node &InsertRight(NodePtr&&) = 0;
		virtual Node const &LeftNode() const = 0;
		virtual Node const &RightNode() const = 0;
		virtual void accept(Visitor&) const = 0;
	};
}
