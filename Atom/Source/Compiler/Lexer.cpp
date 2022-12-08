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

        Interpret(file);
        PostInterpret();

        if (print)
        {
            for (size_t i = 0; i < this->m_Tokens.size(); i++)
            {
                SharedRef<Error> e = Error::Create
                (
                    m_Tokens[i]->GetSpecification().Line,
                    Error::Severity::NO_ERROR,
                    Error::Phase::LEXER
                );
                
                //[FILEPATH][TK][TKSTRING][L:%d-C:%d-S:%d]\n
                e->Message << "[" << m_Tokens[i]->GetSpecification().File.c_str() << "]";
                e->Message << "[" << TokenToString(m_Tokens[i]->GetSpecification().Type) << "]";
                e->Message << "[" << m_Tokens[i]->GetSpecification().String.c_str() << "]";

                e->Message << "[";
                e->Message << "Line:" << e->Line << " ";
                e->Message << "]";
                e->Message << std::endl;
                    
                m_ErrorSystem->AddFeedback(e);
            }
        }
	}

    void Lexer::Interpret(const char* file)
    {
        m_Tokens.clear();

        std::ifstream f(file);

        if (f.bad())
        {
            SharedRef<Error> e = Error::Create(0, Error::Severity::WARNING, Error::Phase::LEXER);
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
                        specs.ResetTo(TokenType::STRING, word, file, line);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);

                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line);

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
                        specs.ResetTo(TokenType::QUOTATION_MARKS, std::string{ '\"' }, file, line);

                        SharedRef<Token> tk = Token::Create(specs);
                        this->m_Tokens.push_back(tk);

                        readingLiterally = true;
                        word.clear();
                        continue;
                    }

                    // verifies range of valid chars
                    int ASCII = std::atoi(std::string{ c }.c_str());

                    bool invalid = false;
                    invalid |= ASCII == 64 || ASCII == 94 || ASCII == 96; // @ ^ `
                    invalid |= ASCII == 35 || ASCII == 36 || ASCII == 38; // # $ ´

                    if (invalid)
                    {
                        SharedRef<Error> e = Error::Create(line, Error::Severity::FATAL, Error::Phase::LEXER);
                        e->Message << "Invalid char " << c << std::endl;
                        m_ErrorSystem->AddError(e);
                    }
                }

                // reading newline or whitespace, process current token and clear
                if (c == ' ' || c == '\n')
                {
                    if (word.size() > 0)
                    {
                        specs.ResetTo(TokenType::STRING, word, file, line);

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
                        specs.ResetTo(TokenType::STRING, word, file, line);

                        SharedRef<Token> tk1 = Token::Create(specs);
                        this->m_Tokens.push_back(tk1);
                    }

                    specs.ResetTo(type, std::string{ c }, file, line);

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
                    specs.ResetTo(type, word, file, line);

                    SharedRef<Token> tk = Token::Create(specs);
                    this->m_Tokens.push_back(tk);

                    word.clear();
                }
            }
        }
    }

    // note: return types must be checked on semantics
    void Lexer::PostInterpret()
    {
        for (size_t i = 0; i < m_Tokens.size(); i++)
        {
            bool variable = false;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::U_32;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::U_64;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::I_32;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::I_64;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::F_32;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::F_64;
            variable |= m_Tokens[i]->GetSpecification().Type == TokenType::CHAR;

            // verifies variable types to address functions
            if (variable && m_Tokens[i-1] != nullptr && m_Tokens[i+1] != nullptr)
            {
                bool change = m_Tokens[i - 1]->GetSpecification().Type == TokenType::FUNCTION;
                change &= m_Tokens[i + 1]->GetSpecification().Type == TokenType::STRING;

                if (change)
                {
                    TokenSpecification specs = m_Tokens[i+1]->GetSpecification();
                    specs.Type = m_Tokens[i - 1]->GetSpecification().Type;
                    m_Tokens[i+1]->SetSpecification(specs);
                }
            }

            // verifies strings to address variable types
            if (m_Tokens[i]->GetSpecification().Type == TokenType::STRING)
            {
                if (m_Tokens[i - 1] != nullptr) // previous exists
                {
                    bool change = false;

                    // var types
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::U_32;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::U_64;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::I_32;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::I_64;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::F_32;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::F_64;
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::CHAR;

                    // imports
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::IMPORT;

                    // defines
                    change |= m_Tokens[i - 1]->GetSpecification().Type == TokenType::DEFINE;

                    if (change)
                    {
                        TokenSpecification specs = m_Tokens[i]->GetSpecification();
                        specs.Type = m_Tokens[i - 1]->GetSpecification().Type;
                        m_Tokens[i]->SetSpecification(specs);
                    }

                    continue;
                }
            }
        }
    }
}