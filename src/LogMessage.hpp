#pragma once
#include "System.hpp"

namespace LogMessage {
	const std::string LogFile = "ARLOG.log.txt";
	static int NbrOfErrors = 0;
	void ErrorMessage(std::string msg, std::string file = "", int line = -1, int chr_pos = -1);
	void ErrorAt(std::string err_msg, int LengthOfToken, std::string err_line, int err_position,
		std::string file = "", int line = -1, std::string token = "", int pos = -1);
	void LogMessage(std::string msg, bool display = true);
	void WarningMessage(std::string msg);
}
