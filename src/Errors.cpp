#include "Errors.hpp"

void Exception::ThrowError(ErrorCodes code, token_t token) {
	LogMessage::ErrorMessage
	(
		"[ E" + std::to_string(code + 1) + " ]" + this->Messages[code],
		token.position.filename,
		token.position.line,
		token.position.char_pos
	);
}
