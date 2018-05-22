#pragma once
#include "System.hpp"

class AST {
private:
	std::string rawcode;
	std::string final;
private:
	/*
	var MyVar : integer = 98;


	*/
public:
	AST(std::string raw) {
		rawcode = raw;
	}
	std::string get() {
		if (!final.empty())
			return final;

	}
};
