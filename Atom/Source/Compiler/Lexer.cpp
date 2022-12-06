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
        m_Tokens.clear();
	}

	void Lexer::Lex(const char* file, bool print)
	{
		LOGGER_TRACE("Lexing %s", file);

        m_Tokens.clear();

		std::ifstream f(file);

		if (f.bad())
		{
			SharedRef<Error> e = Error::Create(0, 0, Error::Severity::WARNING);
			e->Message << "Failed to open file " << file << std::endl;
			m_ErrorSystem->AddError(e);
		}

		if (f.is_open())
		{
			char c{};
			std::string word{};
			bool readingLiterally = false;
			TokenType type = TokenType::INVALID_TOKEN;
			unsigned int line = 1;
			unsigned int scope = 1;
			TokenSpecification specs{};

			// lexer processing
            while (f >> std::noskipws >> c)
            {
                // increment line and reset column on new line
                if (c == '\n' && !readingLiterally)
                {
                    line++;
                }

                // reading literraly
                if (readingLiterally)
                {
                    if (c == '"')
                    {
                        specs.ResetTo(TokenType::STRING, word, file, line, scope);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);

                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line, scope);

                        SharedRef<Token> tk2 = Token::Create(specs);
                        this->m_Tokens.push_back(tk2);

                        word.clear();
                        readingLiterally = false;
                        continue;
                    }

                    word.push_back(c);
                    continue;
                }

                else if (!readingLiterally)
                {
                    if (c == '"')
                    {
                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line, scope);

                        SharedRef<Token> tk = Token::Create(specs);
                        this->m_Tokens.push_back(tk);

                        readingLiterally = true;
                        word.clear();
                        continue;
                    }
                }

                // scopes
                if (c == '{' && !readingLiterally) scope++;
                if (c == '}' && !readingLiterally) scope--;

                // reading newline or whitespace, process current token and clear
                if (c == ' ' || c == '\n')
                {
                    if (word.size() > 0)
                    {
                        specs.Type = TokenType::STRING;
                        specs.String = word;
                        specs.Line = line;
                        specs.Scope = scope;

                        specs.ResetTo(TokenType::STRING, word, file, line, scope);

                        SharedRef<Token> tk = Token::Create(specs);
                        this->m_Tokens.push_back(tk);
                    }

                    word.clear();
                    type = TokenType::INVALID_TOKEN;

                    continue;
                }

                // c is a token itself
                type = StringToToken(std::string{ c }.c_str());
                if (type != TokenType::INVALID_TOKEN)
                {
                    if (word.size() > 0)
                    {
                        specs.ResetTo(TokenType::STRING, word, file, line, scope);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);
                    }

                    specs.ResetTo(type, std::string{ c }, file, line, scope);

                    SharedRef<Token> tk2 = Token::Create(specs);
                    this->m_Tokens.push_back(tk2);

                    word.clear();
                    continue;
                }

                word.push_back(c);

                // word is a valid token
                type = StringToToken(word.c_str());
                if (type != TokenType::INVALID_TOKEN)
                {
                    specs.ResetTo(type, word, file, line, scope);

                    SharedRef<Token> tk = Token::Create(specs);
                    this->m_Tokens.push_back(tk);

                    word.clear();
                }
			}
		}

        if (print)
        {
            for (size_t i = 0; i < this->m_Tokens.size(); i++)
            {
                SharedRef<Error> e = Error::Create
                (
                    m_Tokens[i]->GetSpecification().Line,
                    m_Tokens[i]->GetSpecification().Scope,
                    Error::Severity::NO_ERROR
                );
                
                //[FILEPATH][TK][TKSTRING][L:%d-C:%d-S:%d]\n
                e->Message << "[" << m_Tokens[i]->GetSpecification().File.c_str() << "]";
                e->Message << "[" << TokenToString(m_Tokens[i]->GetSpecification().Type) << "]";
                e->Message << "[" << m_Tokens[i]->GetSpecification().String.c_str() << "]";
                e->Message << "[L:" << e->Line << "-" << "S:" << m_Tokens[i]->GetSpecification().Scope << "]";
                e->Message << std::endl;
                    
                m_ErrorSystem->AddFeedback(e);
            }
        }
	}
}