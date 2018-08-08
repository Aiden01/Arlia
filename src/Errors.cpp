#include "Errors.hpp"

void Exception::ThrowError(ErrorCodes code, token_t token) {
	LogMessage::ErrorMessage
	(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code] + ": " + "'" + token.value + "'",
		token.position.filename,
		token.position.line,
		token.position.char_pos
	);
}
void Exception::ThrowError(ErrorCodes code, token_t token, std::vector<token_t> line) {
	std::string ErrLine;
	for (int i = 0; i < line.size(); ++i) ErrLine += line[i].value + " ";
	LogMessage::ErrorAt
	(
		"[ E" + std::to_string(code) + " ]" + this->Messages[code],
		token.value.size() + token.value.size() - 1,
		ErrLine,
		token.position.char_pos + line.size() - 3,
		token.position.filename,
		token.position.line,
		token.value,
		token.position.char_pos
	);
}
void Exception::ThrowError(ErrorCodes code, std::vector<token_t> line) {
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
