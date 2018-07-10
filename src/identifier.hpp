#pragma once
#include "System.hpp"
#include "statements.h"

namespace identifier {
	void ErrorMessage(std::string ErrValue, std::string CurrentFile = "", int line = -1) {
		LogMessage::ErrorMessage("The identifier is not valid: '" + ErrValue + "'", CurrentFile, line);
	}
	// check if it is a correct identifier name
	bool IsCorrectIdentifier(std::string subject, bool ShowErrMsg = true, std::string CurrentFile = "", int line = -1) {
		bool ret = true;
		if (System::Text::IsVirgin(subject)) ret = false;
		if (keywords::IsKeyword(subject)) ret = false;
		if (System::Text::ContainsSpecialChar(subject)) ret = false;
		if (ret == false && ShowErrMsg) ErrorMessage(subject, CurrentFile, line);
		return ret;
	}
}
