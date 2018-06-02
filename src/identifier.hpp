#pragma once
#include "System.hpp"

namespace identifier {
	void ErrorMessage(std::string ErrValue, int line = -1) {
		System::Display::ErrorMessage("The identifier is not valid: '" + ErrValue + "'", line);
	}
	// check if it is a correct variable name
	bool IsCorrectIdentifier(std::string subject, bool ShowErrMsg = true, int line = -1) {
		bool ret = true;
		if (isdigit(subject[0])) ret = false;
		if (System::Text::ContainsSpecialChar(subject, "_0123456789")) ret = false;
		if (System::Text::IsVirgin(subject)) ret = false;
		if (!ret && ShowErrMsg) ErrorMessage(subject, line);
		return ret;
	}
}
