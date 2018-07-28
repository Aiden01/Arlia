#include "Lexer.hpp"
#include "TokenList.h"
#include "LogMessage.hpp"

/*
	+-------------------------------------------------------------------------------+
	| Lexer has a method that returns the next token of the current stream.         |
	| It also has a function that returns a whole line to an end of line delimiter. |
	| This is what this implementation revolves around.                             |
	+-------------------------------------------------------------------------------+

*/

void SetToken_t(token_t &token, std::string value, TYPE type, location_t pos) {
	token.value = value;
	token.type = type;
	token.position = pos;
}
void SetPosition(location_t &pos, int CurrentCharLineIndex, int line, std::string filename) {
	pos.char_pos = CurrentCharLineIndex;
	pos.line = line;
	pos.filename = filename;
}
bool IsWhiteSpace(char chr) {
	return (chr == ' ' || chr == '\t');
}
bool SetIter(char chr, char next, std::string filename, int CurrentCharIndex, std::string token_tmp) {
	if (IsWhiteSpace(next)) return false;
	std::ifstream file(filename);
	file.seekg(CurrentCharIndex + 1);
	char LongNext = file.peek();
	if (std::isdigit(chr) && next == '.' && std::isdigit(LongNext) || chr == '.' && std::isdigit(next)) return true;
	if (TokenList::IsSymbol(chr) && !TokenList::IsSymbol(chr)) return false;
	if (!TokenList::IsSymbol(chr) && TokenList::IsSymbol(chr)) return false;
	if (next == ';') return false;
	if (chr == '[' && next == ']' || chr == '(' && next == ')' || chr == '{' && next == '}') return false;
	return true;
}
void ManageQuote(char chr, bool &IsInStc, bool &IsDoubleQuote, bool &IsSimpleQuote, bool &IterWhile) {
	if (!IsInStc && chr == '"') IsDoubleQuote = true;
	else if (!IsInStc && chr == '\'') IsSimpleQuote = true;
	else if (IsDoubleQuote && chr == '"') { IsDoubleQuote = false; IterWhile = false; }
	else if (IsSimpleQuote && chr == '\'') { IsSimpleQuote = false; IterWhile = false; }
}
void ImportFaild(std::vector<token_t> line, std::string filename, bool &CanContinue) {
	if (line.size() != 3) {
		size_t length = 0;
		std::string err_line;
		for (token_t token : line) {
			length += token.value.size();
			err_line += token.value + " ";
		}
		LogMessage::ErrorAt("[SE00] Syntax error", length + line.size() - 2, err_line, 0, filename, line[0].position.line);
	}
	else if (!System::File::exist(line[1].value + ".k"))
		LogMessage::ErrorMessage("[HFNF] Header file not found: '" + line[1].value + ".k'", filename, line[0].position.line, line[0].position.char_pos);
	CanContinue = false;
}
void ResetHeader(
	bool &IsInHeader,
	std::ifstream &Header,
	std::ifstream &File,
	int &HeaderLineIndex,
	bool &eof,
	std::string &CurrentFilename,
	std::string &SourceFilename)
{
	IsInHeader = false;
	Header.clear();
	Header.close();
	HeaderLineIndex = 1;
	eof = File.eof();
	if (!eof) CurrentFilename = SourceFilename;
}

Lexer::Lexer(std::string filename) {
	this->File.open(filename);
	if (!this->File.good()) {
		this->File.clear();
		this->CanContinue = false;
		LogMessage::ErrorMessage("Can't load file: '" + filename + "'");
	}
	this->SourceFilename = filename;
	this->CurrentFilename = this->SourceFilename;
	this->CanContinue = true;
	this->eof = this->File.eof();
}
void Lexer::import(std::string filename) {
	this->Header.open(filename);
	if (!this->Header.good()) {
		this->Header.clear();
		this->CanContinue = false;
		LogMessage::ErrorMessage("Can't load file: '" + filename + "'");
		return;
	}
	this->IsInHeader = true;
	this->CurrentFilename = filename;
	this->CanContinue = true;
}
char Lexer::peek() {
	if (this->IsInHeader) return this->Header.peek();
	return this->File.peek();
}
token_t Lexer::next() {
	std::string token_tmp;
	bool IterWhile = true, IsSimpleQuote = false, IsDoubleQuote = false, IsComment = false;
	if (!this->eof)  while (IterWhile) {
		bool IsInStc = (IsDoubleQuote || IsSimpleQuote || IsComment);
		if (this->eof) break; if (this->File.eof() || this->Header.eof()) this->eof = true;
		char chr;
		if (!this->IsInHeader) chr = this->File.get(); else chr = this->Header.get();
		++this->CurrentCharInCurrentLineIndex;
		++this->CurrentCharIndex;
		ManageQuote(chr, IsInStc, IsDoubleQuote, IsSimpleQuote, IterWhile);
		if (!IsInStc && this->peek() == '\n') IterWhile = false;
		if (chr == '\n') {
			if (!this->IsInHeader) ++this->SourceLineIndex; else ++this->HeaderLineIndex;
			this->CurrentCharInCurrentLineIndex = -1;
			continue;
		}
		token_tmp += chr;
		if (!IsInStc) IterWhile = SetIter(chr, this->peek(), this->CurrentFilename, this->CurrentCharIndex, token_tmp);
	}
	if (token_tmp.find_first_not_of(' ') == std::string::npos) return this->next();
	System::Text::trim(token_tmp);
	if (this->eof) token_tmp.pop_back(); // newline problem
	++this->NbrOfTokens;
	location_t pos;
	token_t ret;
	SetPosition(pos, (this->CurrentCharInCurrentLineIndex - token_tmp.size() + 1), this->HeaderLineIndex, this->CurrentFilename);
	SetToken_t(ret, token_tmp, TokenList::ToToken(token_tmp), pos);
	if (ret.type == TokenList::IMPORT) {
		std::vector<token_t> line_imp, tmp;
		line_imp.push_back(ret);
		for (token_t token : this->GetLine()) tmp.push_back(token);
		for (token_t token : tmp) line_imp.push_back(token);
		if (this->CanImport(line_imp)) {
			this->eof = this->Header.eof();
			this->import(line_imp[1].value + ".k");
			return this->next();
		}
		else ImportFaild(line_imp, this->CurrentFilename, this->CanContinue);
	}
	if (this->IsInHeader == true && this->Header.eof())
		ResetHeader(this->IsInHeader, this->Header, this->File, this->HeaderLineIndex, this->eof, this->CurrentFilename, this->SourceFilename);
	if (this->File.eof()) this->CanContinue = false;
	return ret;
}
std::vector<token_t> Lexer::GetLine() {
	std::vector<token_t> ret;
	while (this->peek() != ';' || this->eof) ret.push_back(this->next());
	ret.push_back(this->next());
	return ret;
}
bool Lexer::CanImport(std::vector<token_t> line) {
	// [import] [file] [;]
	/*
	Each header file must end with a [.k] file extension.
	The source file must be [.arl].
	*/
	return
		(
			line.size() == 3 &&
			line[0].type == TokenList::IMPORT &&
			line[1].type == TokenList::IDENTIFIER &&
			line[2].type == TokenList::ENDLINE &&
			System::File::exist(line[1].value + ".k")
			);
}
