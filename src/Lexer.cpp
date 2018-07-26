#include "Lexer.hpp"
#include "TokenList.h"
#include "LogMessage.hpp"

void SetToken_t(token_t *token, std::string value, TYPE type, location_t pos) {
	token->value = value;
	token->type = type;
	token->position = pos;
}
bool IsLangDelim(char chr) {
	return (chr == ' ' || chr == '\t' || TokenList::IsSymbol(System::Text::CharToString(chr)));
}

Lexer::Lexer(std::string filename) {
	this->File.open(filename);
	if (!this->File.good()) {
		this->File.clear();
		LogMessage::ErrorMessage("Can't load file: '" + filename + "'");
		this->CanContinue = false;
	}
	this->CurrentFilename = filename;
	this->CanContinue = true;
	this->Max = System::File::GetFileSize(filename);
}

token_t Lexer::next() {
	token_t ret;
	std::string token_tmp;
	bool IsSimpleQuote = false;	// check string	  --> "..."
	bool IsDoubleQuote = false;	// check char	  --> '...'
	bool IsComment = false;		// check comments --> `...`
	bool IterWhile = true;
	while (IterWhile) {
		bool IsInStc = (IsDoubleQuote || IsSimpleQuote || IsComment);
		std::ifstream file_tmp(this->CurrentFilename);
		if (this->eof) break;
		char chr = this->File.get();
		char next = file_tmp.seekg(this->CurrentCharIndex + 1).get();
		++this->CurrentCharInCurrentLineIndex;
		++this->CurrentCharIndex;
		{
			if (!IsInStc && !IsComment && chr == '`') IsComment = true; else if (!IsInStc && IsComment && chr == '`') { IsComment = false; continue; }
			if (IsComment) continue;
			if (!IsInStc && chr == '"') IsDoubleQuote = true;
			else if (!IsInStc && chr == '\'') IsSimpleQuote = true;
			else if (IsDoubleQuote && chr == '"') IsDoubleQuote = false;
			else if (IsSimpleQuote && chr == '\'') IsSimpleQuote = false;
		}
		if (chr == '\n') {
			++this->CurrentLineIndex;
			this->CurrentCharInCurrentLineIndex = -1;
		}
		token_tmp += chr;
		// if next == lang delim...
		if (!IsInStc && IsLangDelim(chr)) IterWhile = false;
	}
	if (token_tmp.size() > 1 && System::Text::EndsWith(token_tmp, ";") || System::Text::EndsWith(token_tmp, " ")) token_tmp.pop_back();
	++this->NbrOfTokens;
	location_t pos;
	pos.char_pos = this->CurrentCharInCurrentLineIndex;
	pos.filename = this->CurrentFilename;
	pos.line = this->CurrentLineIndex;
	SetToken_t(&ret, token_tmp, TokenList::ToToken(token_tmp), pos);
	return ret;
}

std::vector<token_t> Lexer::GetLine() {
	std::vector<token_t> ret;
	while (this->next().type != TokenList::TokenList::ENDLINE) ret.push_back(this->next());
	return ret;
}

bool Lexer::IsImportSyntax(std::vector<token_t> line) {
	return false;
}
