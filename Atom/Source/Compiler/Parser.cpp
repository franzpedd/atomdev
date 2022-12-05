#include "Parser.h"
#include "Util/Logger.h"

namespace Atom
{
	SharedRef<Parser> Parser::Create(SharedRef<ErrorSystem>& errorsystem)
	{
		return CreateSharedPointer<Parser>(errorsystem);
	}

	Parser::Parser(SharedRef<ErrorSystem>& errorsystem)
		: m_ErrorSystem(errorsystem)
	{
		LOGGER_TRACE("Creating Parser");
	}

	Parser::~Parser()
	{

	}

	void Parser::Parse(const char* file, std::vector<SharedRef<Token>> tokens)
	{
		LOGGER_TRACE("Parsing %s", file);

		SharedRef<Token> last = nullptr;
		SharedRef<Token> current = nullptr;
		SharedRef<Token> next = nullptr;

		if (tokens[0]->GetSpecification().Type != TokenType::IMPORT)
		{
			SharedRef<Error> e = Error::Create(0, 0, Error::Severity::FATAL);
			e->Message << "An import should be the first thing to be made in the language" << std::endl;
			m_ErrorSystem->Add(e);
		}

		for (size_t i = 1; i < tokens.size() - 2; i++)
		{
			SharedRef<Token> current = tokens[i];
			SharedRef<Token> previous = tokens[i-1];
			SharedRef<Token> next = tokens[i+1];
			SharedRef<Error> e = Error::Create();
			
			bool result = IsSequenceValid(current->GetSpecification().Type, previous->GetSpecification().Type, next->GetSpecification().Type);

			if (!result)
			{
				e->Line = current->GetSpecification().Line;
				e->Column = current->GetSpecification().Column;
				e->Level = Error::Severity::FATAL;
				e->Message << "Unexpected Token near %s" << current->GetSpecification().String << std::endl;
				m_ErrorSystem->Add(e);
			}
		}

		if (tokens[tokens.size() - 1]->GetSpecification().Type != TokenType::CLOSE_C_B)
		{
			SharedRef<Error> e = Error::Create(0, 0, Error::Severity::FATAL);
			e->Line = tokens[tokens.size() - 1]->GetSpecification().Line;
			e->Column = tokens[tokens.size() - 1]->GetSpecification().Column;
			e->Level = Error::Severity::FATAL;
			e->Message << "A } is missing/is needed at the end of the code" << std::endl;
			m_ErrorSystem->Add(e);
		}
	}

	bool Parser::IsSequenceValid(TokenType current, TokenType previous, TokenType next)
	{
		bool calculatePrevious = false;
		bool calculateNext = false;

		switch (current)
		{
		case Atom::TokenType::INVALID_TOKEN: break;	
		case Atom::TokenType::NUMBER:
		{
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::NUMBER, TokenType::CLOSE_P, TokenType::CLOSE_B, TokenType::CLOSE_C_B, TokenType::SEMICOLON,			// 0-9 ) ] } ;
				TokenType::COMMA, TokenType::ADDITION,TokenType::SUBTRACTION, TokenType::MULTIPLICATOR,TokenType::DIVISOR,		// , + - * /
				TokenType::AND, TokenType::OR, TokenType::NEGATIVE, TokenType::ASSIGNMENT,TokenType::GREATER_THAN,				// && || ! = >
				TokenType::GREATER_OR_EQUAL_TO, TokenType::LESSER_THAN, TokenType::LESSER_OR_EQUAL_TO, TokenType::EQUIVALENT,	// >= < <= ==
				TokenType::DIFFERENT, TokenType::MODULE																			// != %
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end())
				calculateNext = true;

			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::NUMBER, TokenType::OPEN_P, TokenType::OPEN_B, TokenType::OPEN_C_B, TokenType::SEMICOLON,				// 0-9 ( [ { ;
				TokenType::COMMA, TokenType::ADDITION, TokenType::SUBTRACTION,TokenType::MULTIPLICATOR, TokenType::DIVISOR,		// , + - * /
				TokenType::AND,TokenType::OR, TokenType::NEGATIVE, TokenType::ASSIGNMENT, TokenType::GREATER_THAN,				// && || ! = >
				TokenType::GREATER_OR_EQUAL_TO, TokenType::LESSER_THAN, TokenType::LESSER_OR_EQUAL_TO, TokenType::EQUIVALENT,	// >= < <= ==
				TokenType::DIFFERENT, TokenType::MODULE																			// != %
			};

			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end())
				calculatePrevious = true;
			
			break;
		}

		case Atom::TokenType::STRING: break;
		case Atom::TokenType::I_32:
		case Atom::TokenType::U_32:
		case Atom::TokenType::I_64:
		case Atom::TokenType::U_64:
		case Atom::TokenType::F_32:
		case Atom::TokenType::F_64:
		case Atom::TokenType::CHAR:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING };
			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::OPEN_B, TokenType::CLOSE_B, TokenType::OPEN_C_B, TokenType::CLOSE_C_B,
				TokenType::OPEN_P, TokenType::CLOSE_P, TokenType::SEMICOLON
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::WHILE:
		case Atom::TokenType::FOR:
		case Atom::TokenType::IF:
		case Atom::TokenType::ELSEIF:
		case Atom::TokenType::ELSE:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::OPEN_P };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::OPEN_C_B, TokenType::CLOSE_C_B, TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CASE:
		case Atom::TokenType::DEFAULT:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::COLON };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::SWITCH:
		{
			std::vector<TokenType> acceptableNextTokens { TokenType::OPEN_P};
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;
				
			break;
		}

		case Atom::TokenType::BREAK:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::COLON, TokenType::OPEN_C_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;
				
			break;
		}

		case Atom::TokenType::RETURN:
		{
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::CHAR, TokenType::STRING, TokenType::I_32, TokenType::U_32, TokenType::I_32, TokenType::U_64, TokenType::F_32, 
				TokenType::F_64, TokenType::SEMICOLON
			};
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;
				
			break;
		}

		case Atom::TokenType::CONTINUE:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::SEMICOLON };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B, TokenType::OPEN_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;
				
			break;
		}

		case Atom::TokenType::ENUM:
		case Atom::TokenType::STRUCT:
		case Atom::TokenType::IMPORT:
		case Atom::TokenType::DEFINE:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING, TokenType::OPEN_C_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::FUNCTION:
		{
			std::vector<TokenType> acceptableNextTokens
			{ 
				TokenType::I_32, TokenType::U_32, TokenType::I_64, TokenType::U_64, TokenType::F_32, TokenType::F_64,
				TokenType::CHAR, TokenType::STRING
			};
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::AND:
		case Atom::TokenType::OR:
		case Atom::TokenType::NEGATIVE:
		case Atom::TokenType::ASSIGNMENT:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::NUMBER };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::ADDITION:
		case Atom::TokenType::SUBTRACTION:
		case Atom::TokenType::DIVISOR:
		case Atom::TokenType::MULTIPLICATOR:
		case Atom::TokenType::MODULE:
		case Atom::TokenType::GREATER_THAN:
		case Atom::TokenType::GREATER_OR_EQUAL_TO:
		case Atom::TokenType::LESSER_THAN:
		case Atom::TokenType::LESSER_OR_EQUAL_TO:
		case Atom::TokenType::EQUIVALENT:
		case Atom::TokenType::DIFFERENT:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING, TokenType::NUMBER };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::NUMBER };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::OPEN_B:					// [
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::NUMBER, TokenType::STRING, TokenType::OPEN_B, TokenType::CLOSE_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::OPEN_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CLOSE_B:					// ]
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::EQUIVALENT, TokenType::ASSIGNMENT, TokenType::OPEN_B, TokenType::CLOSE_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::NUMBER, TokenType::OPEN_B, TokenType::CLOSE_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::OPEN_C_B:					// {
		case Atom::TokenType::CLOSE_C_B:				// }
		case Atom::TokenType::OPEN_P:					// (
		{
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::STRING, TokenType::I_32, TokenType::U_32, TokenType::I_64, TokenType::U_64, TokenType::F_32, TokenType::F_64, TokenType::CHAR,
				TokenType::FOR, TokenType::WHILE, TokenType::DEFAULT, TokenType::CASE, TokenType::SWITCH, TokenType::IF, TokenType::ELSEIF, TokenType::ELSE
			};
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::CLOSE_P, TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CLOSE_P:					// )
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::CLOSE_P, TokenType::OPEN_C_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::NUMBER};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::SEMICOLON:
		{
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::STRING, TokenType::I_32, TokenType::U_32, TokenType::I_64, TokenType::U_64, TokenType::F_32, TokenType::F_64, TokenType::CHAR,
				TokenType::FOR, TokenType::WHILE, TokenType::DEFAULT, TokenType::CASE, TokenType::SWITCH, TokenType::IF, TokenType::ELSEIF, TokenType::ELSE,
				TokenType::SEMICOLON
			};

			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::STRING, TokenType::NUMBER, TokenType::CLOSE_B, TokenType::CLOSE_C_B, TokenType::CLOSE_P
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::COLON:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::OPEN_C_B };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::CASE, TokenType::DEFAULT, TokenType::STRING };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::COMMA:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING, TokenType::NUMBER};
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING, TokenType::NUMBER, TokenType::CLOSE_B, TokenType::CLOSE_P };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::DOT:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::STRING, TokenType::NUMBER };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::APOSTROPHE:
		case Atom::TokenType::QUOTATION_MARKS:
		{
			std::vector<TokenType> acceptableNextTokens{ TokenType::CHAR, TokenType::QUOTATION_MARKS, TokenType::SEMICOLON };
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::STRING };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), next) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::COMMENT_L:	return true;
		case Atom::TokenType::COMMENTS_O:	return true;
		case Atom::TokenType::COMMENTS_C:	return true;
		case Atom::TokenType::TOKEN_MAX:	return false;
		default:							return false;
		}

		return calculatePrevious && calculateNext;
	}
}