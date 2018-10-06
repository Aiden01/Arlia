#include "LogMessage.hpp"

void LogMessage::ErrorMessage(std::string msg, std::string file, int line, int chr_pos) {

	System::Display::ColoredMessage(msg + "; at [" + std::to_string(line) + " : "
		+ std::to_string(chr_pos) + "] in '" + file + "'" + "\n", System::Display::color::LightRed);

	System::File::WriteAppend(LogFile, msg + "; at [" + std::to_string(line) + " : "
		+ std::to_string(chr_pos) + "] in '" + file + "'" + "\n");

	// std::cout << std::endl;

	++NbrOfErrors;
}
void LogMessage::ErrorAt(std::string err_msg, int LengthOfToken, std::string err_line, int err_position,
	std::string file, int line, std::string token, int pos) {
	err_msg += ":\n\t" + err_line;
	int indx = 0;
	if (!token.empty()) indx = token.size();

	err_msg += "\n\t";

	for (int i = 1; i < err_line.size(); ++i)
		err_msg += '~';

	err_msg += "\n";
	ErrorMessage(err_msg, file, line, pos);
}
void LogMessage::LogMessage(std::string msg, bool display) {
	if (display) System::Display::LogInfo(msg);
	System::File::WriteAppend(LogFile, "LOG > " + msg + "\n");
}
void LogMessage::WarningMessage(std::string msg) {
	System::Display::LogInfo(msg, System::Display::color::Yellow);
	System::File::WriteAppend(LogFile, "WRN > " + msg + "\n");
}

