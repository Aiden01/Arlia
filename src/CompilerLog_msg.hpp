#pragma once
#include "System.hpp"

namespace LogMessage {
	const std::string LogFile = "ARLOG.log.txt";
	int ErrorCounter = 0;
	void ErrorMessage(std::string msg, int line = -1) {
		System::Display::ErrorMessage(msg, line);
		if (ErrorCounter == 0)
			std::remove(System::Text::StringToCharArray(LogFile));
		System::File::WriteAppend(LogFile, "[" + std::to_string(line) + "] ERROR: " + msg + "." + "\n");
		++ErrorCounter;
	}
	void LogMessage(std::string msg, int line = -1) {
		System::Display::LogInfo(msg);
		System::File::WriteAppend(LogFile, "LOG > " + msg);
	}
}
