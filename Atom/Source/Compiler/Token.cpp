#include "Token.h"

namespace Atom
{
	const char* TokenToString(TokenType token)
	{
        return TokenList[(unsigned int)(token)];
	}

	TokenType StringToToken(std::string str)
	{
        TokenType t = TokenType::INVALID_TOKEN;

        for (size_t i = 0; i < (unsigned int)TokenType::TOKEN_MAX; i++)
        {
            if (str.compare(TokenList[i]) == 0)
            {
                t = (TokenType)(i);
                break;
            }
        }

        return t;
	}

    TokenSpecification::TokenSpecification(TokenType type, std::string string, std::string file, unsigned int line, unsigned int column)
        : Type(type), String(string), File(file), Line(line), Column(column)
    {

    }

    SharedRef<Token> Token::Create(TokenSpecification specs)
    {
        return CreateSharedPointer<Token>(specs);
    }

    Token::Token(TokenSpecification specs)
        : m_Specs(specs)
    {

    }
}