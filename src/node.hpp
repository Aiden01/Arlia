#pragma once
#include "System.hpp"
#include "Parser.hpp"

namespace Parser {

	class Visitor;
	class ConstVisitor;

	class Node {
	public:
		using NodePtr = std::unique_ptr<Node>;
		Node(const Node&) = delete;
		Node &operator=(const Node&) = delete;
		Node() = default;
		virtual ~Node() = default;
		virtual void accept(Visitor&) = 0;
		virtual void analyze(Visitor&) = 0;
		virtual Node &InsertLeft(NodePtr&&) = 0;
		virtual Node &InsertRight(NodePtr&&) = 0;
		virtual Node const &LeftNode() const = 0;
		virtual Node const &RightNode() const = 0;
		virtual void accept(ConstVisitor&) const = 0;
	};

	// | BinaryNode | Data node. Ex: 2 + 3 ('+' with '2' as left and '3' as right)
	class BinaryNode : public Node {
	public:
		using NodePtr = std::unique_ptr<Node>;
		void analyze(Visitor &v);
		Node &InsertLeft(NodePtr NewLeft);
		Node &InsertRight(NodePtr NewRight);
		Node const &LeftNode() const final override;
		Node const &RightNode() const final override;
	private:
		virtual void DoAccept(Visitor &v) = 0;
		NodePtr left_;
		NodePtr right_;
	};

	// | UnaryNode | Operator requiring only one operand to his right. Ex: -5 ('-')
	class UnaryNode : public Node {
	public:
		using NodePtr = std::unique_ptr<Node>;
		void analyze(Visitor &v);
		Node &InsertLeft(NodePtr NewLeft);
		Node const &LeftNode() const;
	private:
		virtual void DoAccept(Visitor &v) = 0;
		NodePtr left_;
	};

	// | Leaf | Data without children. Ex: 2 + 3 ('2' and'3')
	class Leaf : public Node {
	public:
		using NodePtr = std::unique_ptr<Node>;
		void analyze(Visitor &v);
	private:
		virtual void DoAccept(Visitor &v) = 0;
	};

}
