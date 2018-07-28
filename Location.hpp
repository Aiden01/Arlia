#pragma once
#include "System.hpp"

struct location {
	std::string filename;
	int line;
	int char_pos;
};

typedef location location_t;
