#include "node.hpp"

void Parser::BinaryNode::analyze(Parser::Visitor &v) {
	if (left_) left_.get()->analyze(v);
	DoAccept(v);
	if (right_) right_.get()->analyze(v);
}
Parser::Node &Parser::BinaryNode::InsertLeft(Parser::BinaryNode::NodePtr NewLeft) {
	// assert(left_ == nullptr && "Left node already defined");
	left_.reset(NewLeft.release());
	return *(left_.get());
}
Parser::Node &Parser::BinaryNode::InsertRight(Parser::BinaryNode::NodePtr NewRight) {
	// assert(right_ == nullptr && "Right node already defined");
	right_.reset(NewRight.release());
	return *(right_.get());
}
Parser::Node const &Parser::BinaryNode::LeftNode() const {
	// assert(left_ != nullptr && "left node undefined");
	return *(left_.get());
}
Parser::Node const &Parser::BinaryNode::RightNode() const {
	// assert(right_ != nullptr && "right node undefined");
	return *(right_.get());
}

void Parser::UnaryNode::analyze(Parser::Visitor &v) {
	if (left_) left_.get()->analyze(v);
	DoAccept(v);
}
Parser::Node &Parser::UnaryNode::InsertLeft(Parser::UnaryNode::NodePtr NewLeft) {
	// assert(left_ == nullptr && "Left node already defined");
	left_.reset(NewLeft.release());
	return *(left_.get());
}
Parser::Node const &Parser::UnaryNode::LeftNode() const {
	// assert(left_!= nullptr && "left node undefined");
	return *(left_.get());
}

void Parser::Leaf::analyze(Parser::Visitor &v) {
	DoAccept(v);
}

