#pragma once

#include <string>
#include <cstring>

#include "Core/Core.h"
#include "Util/Memory.h"

namespace Atom
{
    enum class API TokenType : unsigned int
    {
        INVALID_TOKEN, NUMBER, LITERAL, VECTOR_CHAR,

        INT, FLOAT, CHAR,
        WHILE, FOR, IF, ELSEIF, ELSE, BREAK, RETURN, CONTINUE,
        ENUM, STRUCT, IMPORT, DEFINE, FUNCTION,

        AND, OR, NEGATIVE, ASSIGNMENT,
        ADDITION, SUBTRACTION, DIVISOR, MULTIPLICATOR, MODULE,
        GREATER_THAN, GREATER_OR_EQUAL_TO, LESSER_THAN, LESSER_OR_EQUAL_TO,
        EQUIVALENT, DIFFERENT,

        OPEN_B, CLOSE_B, OPEN_C_B, CLOSE_C_B, OPEN_P, CLOSE_P,
        SEMICOLON, COMMA, APOSTROPHE, QUOTATION_MARKS,
        COMMENT_L, COMMENTS_O, COMMENTS_C,

        TOKEN_MAX
    };

    static const char* TokenList[] =
    {
        "Invalid Token", "Number", "Literal", "Vector of Chars",

        "int", "float", "char",
        "while", "for", "if", "elif", "else", "break", "return", "continue",
        "enum", "struct", "import", "define", "function",

        "&&", "||", "!", "=",
        "+", "-", "/", "*", "%",
        ">", ">=", "<", "<=",
        "==", "!=",

        "[", "]", "{", "}", "(", ")",
        ";", ",", "'", "\"",
        "--", "/*", "*/",

        "Token Max"
    };

    // stringfies tokentype and returns it
    API const char* TokenToString(TokenType token);

    // tokenizes a string and returns it
    API TokenType StringToToken(std::string str);

    // holds information about a given token
    struct API TokenSpecification
    {
        TokenType Type = TokenType::INVALID_TOKEN;
        std::string String = {};
        std::string File = {};
        unsigned int Line = 0;
        unsigned int Column = 0;

        // constructor
        TokenSpecification() = default;

        // constructor
        TokenSpecification(TokenType type, std::string string, std::string file, unsigned int line, unsigned int column);
    };

    class API Token
    {
    public:

        // returns a smart pointer to a new token
        static SharedRef<Token> Create(TokenSpecification specs);

        // constructor
        Token(TokenSpecification specs);

        // destructor
        ~Token() = default;

    public:

        // returns the token's specification
        inline TokenSpecification& GetSpecification() { return m_Specs; }

        // sets a new token specification
        inline void SetSpecification(TokenSpecification specs) { m_Specs = specs; }

    private:

        TokenSpecification m_Specs;
    };
}