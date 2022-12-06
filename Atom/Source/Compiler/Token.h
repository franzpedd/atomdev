#pragma once

#include <string>
#include <cstring>

#include "Core/Core.h"
#include "Util/Memory.h"

namespace Atom
{
    enum class API TokenType : unsigned int
    {
        INVALID_TOKEN, NUMBER, STRING,

        I_32, U_32, I_64, U_64, F_32, F_64, CHAR,
        WHILE, FOR, IF, ELSEIF, ELSE,
        CASE, DEFAULT, SWITCH, BREAK, RETURN, CONTINUE,
        ENUM, STRUCT, IMPORT, DEFINE, FUNCTION,

        AND, OR, NEGATIVE, ASSIGNMENT,
        ADDITION, SUBTRACTION, DIVISOR, MULTIPLICATOR, MODULE,
        GREATER_THAN, GREATER_OR_EQUAL_TO, LESSER_THAN, LESSER_OR_EQUAL_TO,
        EQUIVALENT, DIFFERENT,

        OPEN_B, CLOSE_B, OPEN_C_B, CLOSE_C_B, OPEN_P, CLOSE_P,
        SEMICOLON, COLON, COMMA, DOT, APOSTROPHE, QUOTATION_MARKS,
        COMMENT_L, COMMENTS_O, COMMENTS_C,

        TOKEN_MAX
    };

    static const char* TokenList[] =
    {
        "Invalid Token", "Number", "String",

        "i32", "u32", "i64", "u64", "f32", "f64", "char",
        "while", "for", "if", "elif", "else",
        "case", "default", "switch", "break", "return", "continue",
        "enum", "struct", "import", "define", "function",

        "&&", "||", "!", "=",
        "+", "-", "/", "*", "%",
        ">", ">=", "<", "<=",
        "==", "!=",

        "[", "]", "{", "}", "(", ")",
        ";", ":", ",", ".", "'", "\"",
        "--", "#begincomm", "#endcomm",

        "Token Max"
    };

    // stringfies tokentype and returns it
    API const char* TokenToString(TokenType token);

    // tokenizes a string and returns it
    API TokenType StringToToken(const char* token);

    // holds information about a given token
    struct API TokenSpecification
    {
        TokenType Type = TokenType::INVALID_TOKEN;
        std::string String = {};
        std::string File = {};
        unsigned int Line = 0;
        unsigned int Scope = 0;

        // constructor
        TokenSpecification() = default;

        // constructor
        TokenSpecification(TokenType type, std::string string, std::string file, unsigned int line, unsigned int scope);

        // resets the token to a given specification
        void ResetTo(TokenType type, std::string string, std::string file, unsigned int line, unsigned int scope);
    };

    class API Token
    {
    public:

        // returns a smart pointer to a new token
        static SharedRef<Token> Create(TokenSpecification specs);

        // constructor
        Token(TokenSpecification specs);

        // destructor
        ~Token();

    public:

        // returns the token's specification
        inline TokenSpecification& GetSpecification() { return this->m_Specs; }

        // sets a new token specification
        inline void SetSpecification(TokenSpecification specs) { this->m_Specs = specs; }

    private:

        TokenSpecification m_Specs;
    };
}