#include "identifier.hpp"
#include "visitor.hpp"

void Parser::Identifier::DoAccept(Parser::Visitor &v) {
	v.visit(*this);
}
