#include "Errors.hpp"
void Exception::ThrowError(ErrorCodes code, std::string token) {
	System::Display::ColoredMessage(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code] + ": " + "'" + token + "'",
		System::Display::color::Yellow
	);
	std::cout << std::endl;
}
void Exception::ThrowError(ErrorCodes code, char token) {
	System::Display::ColoredMessage(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code] + ": " + "'" + std::to_string(short(token)) + "'",
		System::Display::color::Yellow
	);
	std::cout << std::endl;
}
void Exception::ThrowError(ErrorCodes code, token_t token) {
	LogMessage::ErrorMessage
	(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code] + ": " + "'" + token.value + "'",
		token.position.filename,
		token.position.line,
		token.position.char_pos
	);
}
void Exception::ThrowError(ErrorCodes code, token_t token, Expr line) {
	std::string ErrLine;
	for (size_t i = 0; i < line.size(); ++i) ErrLine += line[i].value + " ";
	LogMessage::ErrorAt
	(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code],
		token.value.size() + token.value.size() - 1,
		ErrLine,
		token.position.char_pos + line.size() - 4,
		token.position.filename,
		token.position.line,
		token.value,
		token.position.char_pos
	);
}
void Exception::ThrowError(ErrorCodes code, Expr line) {
	std::string ErrLine;
	for (int i = 0; i < line.size(); ++i) ErrLine += line[i].value + " ";
	LogMessage::ErrorAt
	(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code],
		ErrLine.size() - ErrLine.size() - 1,
		ErrLine,
		line[0].position.char_pos,
		line[0].position.filename,
		line[0].position.line,
		line[0].value,
		line[0].position.char_pos
	);
}
