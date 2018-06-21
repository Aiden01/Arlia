#pragma once
#include "System.hpp"
#include "statements.h"

namespace identifier {
	void ErrorMessage(std::string ErrValue, int line = -1) {
		System::Display::ErrorMessage("The identifier is not valid: '" + ErrValue + "'", line);
	}
	// check if it is a correct identifier name
	bool IsCorrectIdentifier(std::string subject, bool ShowErrMsg = true, int line = -1) {
		if (System::Text::IsVirgin(subject)) return false;
		if (keywords::IsKeyword(subject)) return false;
		if (System::Text::ContainsSpecialChar(subject)) return false;
		return true;
	}
}
