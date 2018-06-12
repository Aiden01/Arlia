#pragma once
#include "System.hpp"
#include "statements.h"

namespace identifier {
	void ErrorMessage(std::string ErrValue, int line = -1) {
		System::Display::ErrorMessage("The identifier is not valid: '" + ErrValue + "'", line);
	}
	// check if it is a correct identifier name
	bool IsCorrectIdentifier(std::string subject, bool ShowErrMsg = true, int line = -1) {
		if (keywords::IsKeyword(subject)) return false;
		if (isdigit(subject[0])) return false;
		if (System::Text::ContainsSpecialChar(subject, "_0123456789")) return false;
		if (System::Text::IsVirgin(subject)) return false;
		return true;
	}
}
