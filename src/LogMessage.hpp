#pragma once
#include "System.hpp"

namespace LogMessage {
	const std::string LogFile = "ARLOG.log.txt";
	static int NbrOfErrors = 0;
	inline void ErrorMessage(std::string msg, std::string file = "", int line = -1, int chr_pos = -1) {
		System::Display::ColoredMessage(msg + "; at [" + std::to_string(line) + " : " + std::to_string(chr_pos) + "] in '" + file + "'" + "\n", System::Display::color::Yellow);
		System::File::WriteAppend(LogFile, msg + "; at [" + std::to_string(line) + " : " + std::to_string(chr_pos) + "] in '" + file + "'" + "\n");
		++NbrOfErrors;
	}
	inline void ErrorAt(std::string err_msg, int LengthOfToken, std::string err_line, int err_position, std::string file = "", int line = -1, int chr_pos = -1) {
		err_msg += ":\n\t" + err_line + "\n\t";
		for (int i = 0; i < err_position; ++i) err_msg += " ";
		for (LengthOfToken; LengthOfToken >= 0; --LengthOfToken) err_msg += "^";
		err_msg += "\n";
		ErrorMessage(err_msg, file, line);
	}
	inline void LogMessage(std::string msg, bool display = true, int line = -1, int chr_pos = -1) {
		if (display) System::Display::LogInfo(msg);
		System::File::WriteAppend(LogFile, "LOG > " + msg + "\n");
	}
	inline void WarningMessage(std::string msg, int line = -1, int chr_pos = -1) {
		System::Display::LogInfo(msg, System::Display::color::Yellow);
		System::File::WriteAppend(LogFile, "WRN > " + msg + "\n");
	}
}
