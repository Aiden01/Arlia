#pragma once
#include "System.hpp"

namespace LogMessage {
	const std::string LogFile = "ARLOG.log.txt";
	int ErrorCounter = 0;
	void ErrorMessage(std::string msg, std::string file = "", int line = -1) {
		System::Display::ErrorMessage(file + " : " + msg, line);
		if (ErrorCounter == 0)
			std::remove(System::Text::StringToCharArray(LogFile));
		System::File::WriteAppend(LogFile, "[" + file + " : " + std::to_string(line) + "] ERROR: " + msg + "." + "\n");
		++ErrorCounter;
	}
	void ErrorAt(std::string err_msg, int LengthOfToken, std::string err_line, int err_position, std::string file = "", int line = -1) {
		err_msg += ":\n\t" + err_line + "\n\t";
		for (int i = 0; i < err_position; ++i) err_msg += " ";
		for (LengthOfToken; LengthOfToken >= 0; --LengthOfToken) err_msg += "^";
		err_msg += "\n";
		ErrorMessage(err_msg, file, line);
	}
	void LogMessage(std::string msg, int line = -1) {
		System::Display::LogInfo(msg);
		System::File::WriteAppend(LogFile, "LOG > " + msg + "\n");
	}
	void WarningMessage(std::string msg, int line = -1) {
		System::Display::LogInfo(msg, System::Display::color::Yellow);
		System::File::WriteAppend(LogFile, "WRN > " + msg + "\n");
	}
}
