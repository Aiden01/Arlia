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
| This is what  implementation revolves around.									|
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
	// -
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
bool CanImport(Expr line, std::vector<std::string> Headers) {
	// [import] [file] [;]
	/*
	Each header file must end with a [.k] file extension.
	The source file must be [.arl].
	*/
	return  (
		line.size() == 3
		&& line[0].type == TokenList::IMPORT
		&& line[1].type == TokenList::IDENTIFIER
		&& line[2].type == TokenList::ENDLINE
		&& !System::Vector::Contains(Headers, line[1].value + ".k")
		&& System::File::exist(line[1].value + HeaderExt)
		);
}
std::string GetImportFilename(Expr line) {
	return line[1].value + HeaderExt;
}

void Lexer::Initialize(std::ifstream &File, std::string &filename_ref, std::string filename, bool &CanContinue, bool &eof, Exception &ex) {
	File.open(filename, std::ios::binary);
	if (!File.good()) {
		File.clear();
		CanContinue = false;
		ex.ThrowError(ex.E0001, filename);
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

Lexer::Lexer(std::string filename) {
	Initialize(File, this->filename, filename, CanContinue, eof, exception);
	StopAferFirstError = StopAferFirstError;
}
// Initializes like one-parameter constructor if not initialised
void Lexer::start(std::string filename) {
	Initialize(File, this->filename, filename, CanContinue, eof, exception);
}
// Reinitializes
void Lexer::end() {
	Terminate(File, filename, eof, CurrentCharIndex, LineIndex, CurrentCharInCurrentLineIndex);
}
// Gets the next char of the current stream without consume it
char Lexer::peekchr() {
	return File.peek();
}
// Returns the next token of the current stream according to certain characteristics
token_t Lexer::next() {
	if (Issnext) {
		Issnext = false;
		token_t ret = snext;
		snext = {};
		return ret;
	}
	if (IsInDefine) {
		if (DefineValue.empty()) IsInDefine = false;
		else {
			token_t ret = DefineValue.front();
			DefineValue.erase(DefineValue.begin());
			return ret;
		}
	}

	std::string token_tmp;
	bool IterWhile = true, IsSimpleQuote = false, IsDoubleQuote = false, IsComment = false;
	if (!eof)  while (IterWhile) {
		bool IsInStc = (IsDoubleQuote || IsSimpleQuote || IsComment);
		if (eof) break;
		if (File.eof()) eof = true;
		char chr = File.get();
		if (!isascii(chr) && chr != EOF) {
			exception.ThrowError(exception.E0076, chr);
			chr = '\0';
		}

		++CurrentCharInCurrentLineIndex;
		++CurrentCharIndex;

		if (chr == '\n') {
			++LineIndex;
			CurrentCharInCurrentLineIndex = 0;
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
		if (!IsInStc && peekchr() == '\n') IterWhile = false;
		token_tmp += chr;
		if (!IsInStc) IterWhile = SetIter(chr, peekchr(), IsInStc, filename, CurrentCharIndex, token_tmp);
	}

	if (token_tmp.find_first_not_of(' ') == std::string::npos) return next();
	System::Text::trim(token_tmp);
	if (eof)  // Causes a new unexpected empty line
		token_tmp.pop_back();
	location_t pos;
	token_t ret;
	SetPosition(pos, (CurrentCharInCurrentLineIndex - token_tmp.size() + 1), LineIndex, filename);
	SetToken_t(ret, token_tmp, TokenList::ToToken(token_tmp), pos);
	if (File.eof()) CanContinue = false;
	if (ret.type == TokenList::NOTHING && CanContinue) return next();

	if (Define.IsDefined(ret.value)) {
		DefineValue = Define.get(ret.value);
		IsInDefine = true;
		return next();
	}

	if (ret.type == TokenList::RIGHT_BRACE) {
		/*Issnext = true;
		snext = ret;
		ret = { TokenList::ENDLINE, ";", snext.position };*/
	}

	return ret;
}
// Returns the next token of the stream without consuming it
token_t Lexer::peekt() {
	if (eof) return { TokenList::NOTHING };
	token_t ret = next();
	Issnext = true;
	snext = ret;
	return ret;
}
// GetUntil is used to get a suite of token until a specific token
Expr Lexer::GetUntil(TokenList::TokenList until, bool included) {
	Expr ret;
	token_t token;
	do {
		if (!eof) {
			token = next();
			ret.push_back(token);
		}
	} while (token.type != until && peekchr() != EOF);
	if (!included && !eof) {
		Issnext = true;
		snext = ret.back();
		ret.pop_back();
	}
	if (ret.back().type == TokenList::TokenList::NOTHING && !eof) ret.pop_back();
	return ret;
}
// GetLine is used to get a suite of token ending with an ENDLINE (';') token
Expr Lexer::GetLine(bool keepit) {
	return GetUntil(TokenList::TokenList::ENDLINE, keepit);
}
