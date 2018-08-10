#include "Parser.hpp"
#include "Expression.hpp"

void Parser::parser::parse
(Lexer &lexer, size_t &NbrOfToken, std::vector<std::string> &HeaderImported, Exception &exception, std::string HeaderFilename) {
	Lexer LocalLexer;
	token_t token;
	bool IsHeader = (!HeaderFilename.empty());
	if (IsHeader) LocalLexer.start(HeaderFilename);

	if (IsHeader && !System::Vector::Contains(HeaderImported, HeaderFilename)) HeaderImported.push_back(HeaderFilename);

	while (true) {
		if (!IsHeader && (lexer.eof || !lexer.CanContinue)) break;
		else if (IsHeader && (LocalLexer.eof || !LocalLexer.CanContinue)) break;
		if (IsHeader) token = LocalLexer.next();
		else token = lexer.next();

		Expr line{ token }; // 'token' is the first item from the potential line
		TokenList::TokenList TokenType = token.type;

		std::cout << token.value << std::endl;

		switch (TokenType) {
		case TokenList::VAR:
			break;
		case TokenList::FUNC:
			break;
		case TokenList::RET:
			break;
		case TokenList::INSTANCE:
			break;
		case TokenList::ENUM:
			break;
		case TokenList::NAMESPACE:
			break;
		case TokenList::WHILE:
			break;
		case TokenList::FOR:
			break;
		case TokenList::TO:
			break;
		case TokenList::_IN:
			break;
		case TokenList::STEP:
			break;
		case TokenList::STRUCTURE:
			break;
		case TokenList::RETURN:
			break;
		case TokenList::CONTINUE:
			break;
		case TokenList::SET:
			break;
		case TokenList::AWAIT:
			break;
		case TokenList::IF:
			break;
		case TokenList::ELIF:
			break;
		case TokenList::ELSE:
			break;
		case TokenList::MATCH:
			break;
		case TokenList::CASE:
			break;
		case TokenList::DEFAULT:
			break;
		case TokenList::EXTERN:
			break;
		case TokenList::THROW:
			break;
		case TokenList::TRY:
			break;
		case TokenList::CATCH:
			break;
		case TokenList::EACH:
			break;
		case TokenList::_TRUE:
			break;
		case TokenList::_FALSE:
			break;
		case TokenList::IS:
			break;
		case TokenList::ISNT:
			break;
		case TokenList::IT:
			break;
		case TokenList::PUBLIC:
			break;
		case TokenList::PRIVATE:
			break;
		case TokenList::STATIC:
			break;
		case TokenList::UPON:
			break;
		case TokenList::LONG_RIGHT_ARROW:
			break;
		case TokenList::INSTANCE_ENUM_STRUCTURE_OBJCALL:
			break;
		case TokenList::WITH:
			break;
		case TokenList::AND:
			break;
		case TokenList::OR:
			break;
		case TokenList::PLUS:
			break;
		case TokenList::MINUS:
			break;
		case TokenList::TIME:
			break;
		case TokenList::DIVIDE:
			break;
		case TokenList::MODULO:
			break;
		case TokenList::INC:
			break;
		case TokenList::DEC:
			break;
		case TokenList::ISEQUAL:
			break;
		case TokenList::EQUAL:
			break;
		case TokenList::LESS:
			break;
		case TokenList::GREATER:
			break;
		case TokenList::LESSEQ:
			break;
		case TokenList::GREATEREQ:
			break;
		case TokenList::ISNOTEQUAL:
			break;
		case TokenList::OPPOSITE:
			break;
		case TokenList::PLUS_EQUAL:
			break;
		case TokenList::LESS_EQUAL:
			break;
		case TokenList::TIME_EQUAL:
			break;
		case TokenList::DIVIDE_EQUAL:
			break;
		case TokenList::MODULO_EQUAL:
			break;
		case TokenList::CONCAT:
			break;
		case TokenList::CONCAT_EQUAL:
			break;
		case TokenList::CONCAT_PLUS:
			break;
		case TokenList::CONCAT_LESS:
			break;
		case TokenList::CONCAT_TIME:
			break;
		case TokenList::CONCAT_DIVIDE:
			break;
		case TokenList::CONCAT_MODULO:
			break;
		case TokenList::LEFT_HOOK:
			break;
		case TokenList::RIGHT_HOOK:
			break;
		case TokenList::LEFT_BRACE:
			break;
		case TokenList::RIGHT_BRACE:
			break;
		case TokenList::LEFT_PARENTHESIS:
			break;
		case TokenList::RIGHT_PARENTHESIS:
			break;
		case TokenList::DOT:
			break;
		case TokenList::NAMESPACE_CALLING:
			break;
		case TokenList::COMMA:
			break;
		case TokenList::ENDLINE:
			break;
		case TokenList::POINTER_ADRESS:
			break;
		case TokenList::POINTER_VALUE:
			break;
		case TokenList::NEW:
			break;
		case TokenList::_DELETE:
			break;
		case TokenList::FREE:
			break;
		case TokenList::IMPORT:
			if (IsHeader) System::Vector::push_range(line, LocalLexer.GetLine());
			else System::Vector::push_range(line, lexer.GetLine());
			if (CanImport(line)) this->parse(LocalLexer, NbrOfToken, HeaderImported, exception, GetImportFilename(line));
			else ImportFailed(line, token.position.filename, exception, lexer.CanContinue, lexer.StopAferFirstError);
			NbrOfToken += line.size() - 1; // because process is sent to parse()
			break;
		case TokenList::DEFINE:
			break;
		case TokenList::IFDEF:
			break;
		case TokenList::ELIFDEF:
			break;
		case TokenList::GOTO:
			break;
		case TokenList::PROC:
			break;
		case TokenList::SIZEOF:
			break;
		case TokenList::NUMBER:
			break;
		case TokenList::CHAR:
			break;
		case TokenList::STRING:
			break;
		case TokenList::TYPENAME:
			break;
		case TokenList::TYPESIZE:
			break;
		case TokenList::IDENTIFIER:
			break;
		case TokenList::UNKNOWN:
			exception.ThrowError(exception.E0018, token);
			break;
		case TokenList::NOTHING:
			break;
		}
		++NbrOfToken;
	}
}
