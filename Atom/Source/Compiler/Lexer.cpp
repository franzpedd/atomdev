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
                    m_Tokens[i]->GetSpecification().File,
                    m_Tokens[i]->GetSpecification().Line,
                    m_Tokens[i]->GetSpecification().Column,
                    Error::Severity::NO_ERROR,
                    Error::Phase::LEXER
                );

                e->Message << "[" << TokenToString(m_Tokens[i]->GetSpecification().Type) << "]: ";
                e->Message << m_Tokens[i]->GetSpecification().String;
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
            SharedRef<Error> e = Error::Create(file, 0, 0, Error::Severity::WARNING, Error::Phase::LEXER);
            e->Message << "Falha ao abrir arquivo" << std::endl;
            m_ErrorSystem->AddError(e);
        }

        if (f.is_open())
        {
            char c = {};
            unsigned int line = 1;
            unsigned int column = 0;
            std::string literal = {};
            bool readingLiterally = false;
            bool readingCommentLine = false;
            bool readingCommentMultiLine = false;
            bool auxCommentMultiLine = false;
            TokenType type = TokenType::INVALID_TOKEN;

            while (f >> std::noskipws >> c)
            {
                column++;
                if (c == '\n' && !readingLiterally)
                {
                    line++;
                    column = 0;
                    readingCommentLine = false;
                }

                // singleline comment
                if (c == '-' && f.peek() == '-' && !readingLiterally) readingCommentLine = true;
                if (readingCommentLine) continue;

                // multiline comment
                if (c == '/' && f.peek() == '*' && !readingLiterally) readingCommentMultiLine = true;
                if (c == '*' && f.peek() == '/' && !readingLiterally)
                {
                    readingCommentMultiLine = false;
                    f.ignore();
                    continue;
                }

                if (readingCommentMultiLine) continue;

                // stops reading literally
                if (readingLiterally)
                {
                    if (c == '"')
                    {
                        literal.push_back(c);

                        SharedRef<Token> tk = Token::Create(TokenSpecification(TokenType::VECTOR_CHAR, literal, file, line, column - (unsigned int)literal.size() + 1));
                        m_Tokens.push_back(tk);

                        literal.clear();
                        readingLiterally = false;

                        continue;
                    }

                    literal.push_back(c);
                    continue;
                }

                // starts reading literally
                else if (!readingLiterally)
                {
                    if (c == '"')
                    {
                        literal.clear();
                        literal.push_back(c);
                        readingLiterally = true;
                        continue;
                    }
                }

                // see if c is invalid
                if (c == '$' || c == '&' || c == '@' || c == '^' || c == '`' || c == '~')
                {
                    SharedRef<Error> e = Error::Create(file, line, column, Error::Severity::FATAL, Error::Phase::LEXER);
                    e->Message << "Caractere invalido " << c << std::endl;
                    m_ErrorSystem->AddError(e);
                }

                // reading space or newline, process token
                if (c == ' ' || c == '\n')
                {
                    if (!literal.empty())
                    {
                        SharedRef<Token> tk = Token::Create(TokenSpecification(TokenType::LITERAL, literal, file, line, column));
                        m_Tokens.push_back(tk);
                    }

                    literal.clear();
                    continue;
                }

                // c is a token
                type = StringToToken(std::string{c});
                if (type != TokenType::INVALID_TOKEN)
                {
                    if (!literal.empty())
                    {
                        SharedRef<Token> tk1 = Token::Create(TokenSpecification(TokenType::LITERAL, literal, file, line, column - (unsigned int)literal.size()));
                        m_Tokens.push_back(tk1);
                    }

                    SharedRef<Token> tk2 = Token::Create(TokenSpecification(type, std::string{c}, file, line, column));
                    m_Tokens.push_back(tk2);

                    literal.clear();
                    continue;
                }

                // literal is a token
                literal.push_back(c);
                type = StringToToken(literal);

                if (type != TokenType::INVALID_TOKEN)
                {
                    SharedRef<Token> tk = Token::Create(TokenSpecification(type, TokenToString(type), file, line, column - (unsigned int)literal.size() + 1));
                    m_Tokens.push_back(tk);

                    literal.clear();
                }
            }
        }
    }

    // converts numbers to float if contains . beetween then
    // converts literals to numbers if they are numbers
    void Lexer::PostInterpret()
    {
        std::vector<SharedRef<Token>> tokens;

        for (size_t i = 0; i < m_Tokens.size(); i++)
        {
            if (m_Tokens[i]->GetSpecification().Type == TokenType::LITERAL)
            {
                bool isNumber = StringIsNumber(m_Tokens[i]->GetSpecification().String);

                if (isNumber)
                {
                    m_Tokens[i]->GetSpecification().Type = TokenType::NUMBER;
                    continue;
                }

                bool isFloat = StringIsFloat(m_Tokens[i]->GetSpecification().String);

                if (isFloat)
                {
                    m_Tokens[i]->GetSpecification().Type = TokenType::FLOAT;
                    continue;
                }
            }
        }
    }

    bool Lexer::StringIsNumber(std::string str)
    {
        bool result = true;
        for (size_t i = 0; i < str.size(); i++)
        {
            if (str[i] >= '0' && str[i] <= '9') continue;
            else result = false;
        }

        return result;
    }

    bool Lexer::StringIsFloat(std::string str)
    {
        bool result = true;
        unsigned int dots = 0;

        for (size_t i = 0; i < str.size(); i++)
        {
            if (str[i] == '.')
            {
                dots++;
                continue;
            }

            if (str[i] >= '0' && str[i] <= '9') continue;
            else result = false;
        }

        return result && dots == 1;
    }

}