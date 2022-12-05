#include "Lexer.h"
#include "Util/Logger.h"

namespace Atom
{
	SharedRef<Lexer> Lexer::Create(SharedRef<ErrorSystem>& errorsystem)
	{
		return CreateSharedPointer<Lexer>(errorsystem);
	}

	Lexer::Lexer(SharedRef<ErrorSystem>& errorsystem)
		: m_ErrorSystem(errorsystem)
	{
		LOGGER_TRACE("Creating the Lexer");
	}

	Lexer::~Lexer()
	{

	}

	void Lexer::Lex(const char* file, bool print)
	{
		LOGGER_TRACE("Lexing %s", file);

		std::ifstream f(file);

		if (f.bad())
		{
			SharedRef<Error> e = Error::Create(0, 0, Error::Severity::WARNING);
			e->Message << "Failed to open file " << file << std::endl;
			m_ErrorSystem->Add(e);
		}

		if (f.is_open())
		{
			char c{};
			std::string word{};
			bool readingLiterally = false;
			TokenType type = TokenType::INVALID_TOKEN;
			unsigned int line = 1;
			unsigned int column = 1;
			unsigned int scope = 1;
			TokenSpecification specs{};

			// lexer processing
            while (f >> std::noskipws >> c)
            {
                // increment line and reset column on new line
                if (c == '\n' && !readingLiterally)
                {
                    line++;
                    column = 0;
                }

                // reading literraly
                if (readingLiterally)
                {
                    if (c == '"')
                    {
                        specs.ResetTo(TokenType::STRING, word, file, line, column - (unsigned int)word.size(), scope);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);

                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line, (column - (unsigned int)word.size()) + 1, scope);

                        SharedRef<Token> tk2 = Token::Create(specs);
                        this->m_Tokens.push_back(tk2);

                        word.clear();
                        column++;
                        readingLiterally = false;
                        continue;
                    }

                    word.push_back(c);
                    column++;
                    continue;
                }

                else if (!readingLiterally)
                {
                    if (c == '"')
                    {
                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line, (column - (unsigned int)word.size()) + 1, scope);

                        SharedRef<Token> tk = Token::Create(specs);
                        this->m_Tokens.push_back(tk);

                        readingLiterally = true;
                        word.clear();
                        column++;
                        continue;
                    }
                }

                // reading newline or whitespace, process current token and clear
                if (c == ' ' || c == '\n')
                {
                    if (word.size() > 0)
                    {
                        specs.Type = TokenType::STRING;
                        specs.String = word;
                        specs.Line = line;
                        specs.Column = column - (unsigned int)word.size();
                        specs.Scope = scope;

                        specs.ResetTo(TokenType::STRING, word, file, line, column - (unsigned int)word.size(), scope);

                        SharedRef<Token> tk = Token::Create(specs);
                        this->m_Tokens.push_back(tk);
                    }

                    word.clear();
                    type = TokenType::INVALID_TOKEN;
                    column++;

                    continue;
                }

                // scopes
                if (c == '{' && !readingLiterally) scope++;
                if (c == '}' && !readingLiterally) scope--;

                // c is a token itself
                type = StringToToken(std::string{ c }.c_str());
                if (type != TokenType::INVALID_TOKEN)
                {
                    if (word.size() > 0)
                    {
                        specs.ResetTo(TokenType::STRING, word, file, line, column - (unsigned int)word.size(), scope);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);
                    }

                    specs.ResetTo(type, std::string{ c }, file, line, (column - (unsigned int)word.size()) + 1, scope);

                    SharedRef<Token> tk2 = Token::Create(specs);
                    this->m_Tokens.push_back(tk2);

                    column++;
                    word.clear();
                    continue;
                }

                word.push_back(c);

                // word is a valid token
                type = StringToToken(word.c_str());
                if (type != TokenType::INVALID_TOKEN)
                {
                    specs.ResetTo(type, word, file, line, (column - (unsigned int)word.size()) + 1, scope);

                    SharedRef<Token> tk = Token::Create(specs);
                    this->m_Tokens.push_back(tk);

                    word.clear();
                }

                column++;
			}
		}

        if (print)
        {
            for (size_t i = 0; i < this->m_Tokens.size(); i++)
            {
                LOGGER_TRACE("[%d:%d:%d][FILE:%s][TK:%s][STR:%s]",
                    m_Tokens[i]->GetSpecification().Scope,
                    m_Tokens[i]->GetSpecification().Line,
                    m_Tokens[i]->GetSpecification().Column,
                    m_Tokens[i]->GetSpecification().File.c_str(),
                    TokenToString(m_Tokens[i]->GetSpecification().Type),
                    m_Tokens[i]->GetSpecification().String.c_str()
                );
            }
        }
	}
}