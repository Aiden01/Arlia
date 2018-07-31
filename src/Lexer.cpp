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
void ImportFailed(std::vector<token_t> line, std::string filename, bool &CanContinue) {
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
// Checks if a suite match to an import syntax and that file to import exist
bool CanImport(std::vector<token_t> line) {
	// [import] [file] [;]
	/*
	Each header file must end with a [.k] file extension.
	The source file must be [.arl].
	*/
	return  (
		line.size() == 3 &&
		line[0].type == TokenList::IMPORT &&
		line[1].type == TokenList::IDENTIFIER &&
		line[2].type == TokenList::ENDLINE &&
		System::File::exist(line[1].value + HeaderExt)
		);
}
std::string GetImportFilename(std::vector<token_t> line) {
	return line[1].value + HeaderExt;
}

void Initialize(std::ifstream &File, std::string &filename_ref, std::string filename, bool &CanContinue, bool &eof) {
	File.open(filename);
	if (!File.good()) {
		File.clear();
		CanContinue = false;
		LogMessage::ErrorMessage("[CLF0]: Can't load file: '" + filename + "'");
	}
	filename_ref = filename;
	CanContinue = true;
	eof = File.eof();
}
void Terminate(std::ifstream &File) {
	File.close();
}
void Terminate(std::ifstream &File, std::string &filename, bool &eof, int &CurrentCharIndex, int &LineIndex, int &CurrentCharInCurrentLineIndex) {
	File.close();
	filename.clear();
	eof = false;
	CurrentCharIndex = 0;
	LineIndex = 1;
	CurrentCharInCurrentLineIndex = -1;
}

/*** Lexer implementation ***/

// Initializes like one-parameter constructor if not initialised
void Lexer::start(std::string filename) {
	Initialize(this->File, this->filename, filename, this->CanContinue, this->eof);
}
// Reinitializes
void Lexer::end() {
	Terminate(this->File, this->filename, this->eof, this->CurrentCharIndex, this->LineIndex, this->CurrentCharInCurrentLineIndex);
}
// One-parameter constructor 
Lexer::Lexer(std::string filename) {
	Initialize(this->File, this->filename, filename, this->CanContinue, this->eof);
}
// Gets the next char of the current stream without consume it
char Lexer::peekchr() {
	return this->File.peek();
}
// Returns the next token of the current stream according to certain characteristics
token_t Lexer::next() {
	std::string token_tmp;
	bool IterWhile = true, IsSimpleQuote = false, IsDoubleQuote = false, IsComment = false;
	if (!this->eof)  while (IterWhile) {
		bool IsInStc = (IsDoubleQuote || IsSimpleQuote || IsComment);
		if (this->eof) break;
		if (this->File.eof()) this->eof = true;
		char chr = this->File.get();
		++this->CurrentCharInCurrentLineIndex;
		++this->CurrentCharIndex;
		ManageQuote(chr, IsInStc, IsDoubleQuote, IsSimpleQuote, IterWhile);
		if (!IsInStc && this->peekchr() == '\n') IterWhile = false;
		if (chr == '\n') {
			++this->LineIndex;
			this->CurrentCharInCurrentLineIndex = -1;
			continue;
		}
		token_tmp += chr;
		if (!IsInStc) IterWhile = SetIter(chr, this->peekchr(), this->filename, this->CurrentCharIndex, token_tmp);
	}
	if (token_tmp.find_first_not_of(' ') == std::string::npos) return this->next();
	System::Text::trim(token_tmp);
	if (this->eof)  // Causes a new unexpected empty line
		token_tmp.pop_back();
	++this->NbrOfTokens;
	location_t pos;
	token_t ret;
	SetPosition(pos, (this->CurrentCharInCurrentLineIndex - token_tmp.size() + 1), this->LineIndex, this->filename);
	SetToken_t(ret, token_tmp, TokenList::ToToken(token_tmp), pos);
	if (this->File.eof()) this->CanContinue = false;
	return ret;
}
// GetLine is used to get a suite of token ending with an ENDLINE token
std::vector<token_t> Lexer::GetLine() {
	std::vector<token_t> ret;
	while (this->peekchr() != ';' || this->eof) ret.push_back(this->next());
	ret.push_back(this->next());
	return ret;
}
