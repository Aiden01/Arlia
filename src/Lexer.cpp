#include "Lexer.hpp"
#include "TokenList.h"
#include "Errors.hpp"

#define StopIter	  return false // Stop current loop iteration
#define ContinueIter  return true  // Continue current loop iteration

#define Continue	  return false // Termine loop iteration
#define Jump		  return true  // Jump to next loop iteration

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
bool IsIdentifierChar(char chr) {
	// Only letters and numbers
	return std::isalnum(chr);
}
bool SetIter(char chr, char next, bool IsInStc, std::string filename, int CurrentCharIndex, std::string token_tmp) {
	const std::array<char, 8> balises = { '(', ')', '[', ']', '{', '}', '<', '>' };
	std::ifstream file(filename);
	file.seekg(CurrentCharIndex + 1);
	char LongNext = file.peek();
	// -->
	if (chr == '-' && next == '-' && LongNext == '>') ContinueIter;
	if (chr == '-' && next == '>') ContinueIter;

	if (next == '\n') StopIter;
	if (next == EOF) StopIter;

	if (std::find(balises.begin(), balises.end(), next) != std::end(balises)) StopIter;
	if (std::find(balises.begin(), balises.end(), chr) != std::end(balises)) StopIter;
	if (IsWhiteSpace(next)) StopIter;
	if (std::isdigit(chr) && next == '.' && std::isdigit(LongNext) || chr == '.' && std::isdigit(next)) ContinueIter;

	if ((std::isdigit(chr) || TokenList::IsIdentifier(System::Text::CharToString(chr))) && TokenList::IsSymbol(next))
		StopIter;
	
	if (TokenList::IsSymbol(chr) && !TokenList::IsSymbol(next)) StopIter;
	if (!TokenList::IsSymbol(next) && TokenList::IsSymbol(chr)) StopIter;
	if (next == ';' || chr == ';' || next == ',' || chr == ',' || chr == '@' || chr == ':' || next == ':') StopIter;
	if (chr == '.') StopIter;
	if (IsIdentifierChar(chr) && next == '.') StopIter;
	ContinueIter;
}
void ManageQuote(char chr, bool &IsInStc, bool &IsDoubleQuote, bool &IsSimpleQuote, bool &IterWhile) {
	if (!IsInStc && chr == '"') IsDoubleQuote = true;
	else if (!IsInStc && chr == '\'') IsSimpleQuote = true;
	else if (IsDoubleQuote && chr == '"') { IsDoubleQuote = false; IterWhile = false; }
	else if (IsSimpleQuote && chr == '\'') { IsSimpleQuote = false; IterWhile = false; }
}
void ImportFailed(Expr line, std::string filename, Exception &exception, bool &CanContinue, bool StopAferFirstError) {
	if (line.size() != 3) {
		exception.ThrowError(exception.E0062, line[0]);
		exception.ThrowError(exception.E0002, line[0]);
	}
	else if (!System::File::exist(line[1].value + ".k")) exception.ThrowError(exception.E0002, line[1]);
	if (StopAferFirstError) CanContinue = false;
}
// Checks if a suite match to an import syntax and that file to import exist
bool CanImport(Expr line) {
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
std::string GetImportFilename(Expr line) {
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
Lexer::Lexer(std::string filename, bool StopAferFirstError = false) {
	Initialize(this->File, this->filename, filename, this->CanContinue, this->eof);
	this->StopAferFirstError = StopAferFirstError;
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
		if (!isascii(chr) && chr != EOF) {
			this->exception.ThrowError(this->exception.E0076, chr);
			chr = '\0';
		}

		++this->CurrentCharInCurrentLineIndex;
		++this->CurrentCharIndex;

		if (chr == '\n') {
			++this->LineIndex;
			this->CurrentCharInCurrentLineIndex = -1;
			continue;
		}

		if (IsComment && chr != '`') continue;
		else if (IsComment && chr == '`') {
			IsComment = false;
			continue;
		}
		if (!IsInStc && chr == '`') {
			IsComment = true;
			continue;
		}

		ManageQuote(chr, IsInStc, IsDoubleQuote, IsSimpleQuote, IterWhile);
		if (!IsInStc && this->peekchr() == '\n') IterWhile = false;
		token_tmp += chr;
		if (!IsInStc) IterWhile = SetIter(chr, this->peekchr(), IsInStc, this->filename, this->CurrentCharIndex, token_tmp);
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
Expr Lexer::GetLine() {
	Expr ret;
	while (this->peekchr() != ';' && this->peekchr() != EOF) ret.push_back(this->next());
	if (!this->eof) ret.push_back(this->next());
	if (ret.back().type == TokenList::TokenList::NOTHING) ret.pop_back();
	return ret;
}
// GetUntil is used to get a suite of token until a specific token
Expr Lexer::GetUntil(TokenList::TokenList until, bool included) {
	Expr ret;
	token_t token;
	do {
		if (!this->eof) {
			token = this->next();
			ret.push_back(token);
		}
	} while (token.type != until && this->peekchr() != EOF);
	if (!included) ret.pop_back();
	if (ret.back().type == TokenList::TokenList::NOTHING) ret.pop_back();
	return ret;
}
