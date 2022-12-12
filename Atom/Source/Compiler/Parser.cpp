#include "Parser.h"
#include "Util/Logger.h"

#include <algorithm>

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

		if (tokens.size() < 3)
		{
			SharedRef<Error> e = Error::Create
			(
				file,
				0,
				0,
				Error::Severity::FATAL,
				Error::Phase::PARSER
			);
			e->Message << "Impossivel realizar o parsing por falta de tokens" << std::endl;
			m_ErrorSystem->AddError(e);

			return;
		}

		SharedRef<Token> previous = nullptr;
		SharedRef<Token> current = nullptr;
		SharedRef<Token> next = nullptr;
		
		for (size_t i = 1; i < tokens.size() - 1; i++)
		{
			current = tokens[i];
			previous = tokens[i - 1];
			next = tokens[i + 1];

			bool result = IsSequenceValid(current->GetSpecification().Type, previous->GetSpecification().Type, next->GetSpecification().Type);

			if (!result)
			{
				SharedRef<Error> e = Error::Create
				(
					file,
					tokens[i]->GetSpecification().Line,
					tokens[i]->GetSpecification().Column,
					Error::Severity::FATAL,
					Error::Phase::PARSER
				);
				e->Message << "Sequencia de tokens invalidos: " 
					<< previous->GetSpecification().String << " " 
					<< current->GetSpecification().String << " " 
					<< next->GetSpecification().String << std::endl;
				m_ErrorSystem->AddError(e);
			}

			else
			{
				SharedRef<Error> e = Error::Create
				(
					tokens[i]->GetSpecification().File,
					tokens[i]->GetSpecification().Line,
					tokens[i]->GetSpecification().Column,
					Error::Severity::NO_ERROR,
					Error::Phase::PARSER
				);
				e->Message << "Sequencia de tokens invalidos: "
					<< previous->GetSpecification().String << " "
					<< current->GetSpecification().String << " "
					<< next->GetSpecification().String << std::endl;
				m_ErrorSystem->AddFeedback(e);
			}
		}

		if (tokens[tokens.size() - 1]->GetSpecification().Type != TokenType::CLOSE_C_B)
		{
			SharedRef<Error> e = Error::Create
			(
				file,
				tokens[tokens.size() - 1]->GetSpecification().Line,
				tokens[tokens.size() - 1]->GetSpecification().Column,
				Error::Severity::FATAL,
				Error::Phase::PARSER
			);
			e->Message << "Expected token } but found" << tokens[tokens.size() - 1]->GetSpecification().String << std::endl;
			m_ErrorSystem->AddError(e);
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
			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::NUMBER, TokenType::OPEN_P, TokenType::OPEN_B, TokenType::OPEN_C_B, TokenType::SEMICOLON,				// 0-9 ( [ { ;
				TokenType::COMMA, TokenType::ADDITION, TokenType::SUBTRACTION,TokenType::MULTIPLICATOR, TokenType::DIVISOR,		// , + - * /
				TokenType::AND,TokenType::OR, TokenType::NEGATIVE, TokenType::ASSIGNMENT, TokenType::GREATER_THAN,				// && || ! = >
				TokenType::GREATER_OR_EQUAL_TO, TokenType::LESSER_THAN, TokenType::LESSER_OR_EQUAL_TO, TokenType::EQUIVALENT,	// >= < <= ==
				TokenType::DIFFERENT, TokenType::MODULE, TokenType::RETURN														// != % return
			};
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::NUMBER, TokenType::CLOSE_P, TokenType::CLOSE_B, TokenType::CLOSE_C_B, TokenType::SEMICOLON,			// 0-9 ) ] } ;
				TokenType::COMMA, TokenType::ADDITION,TokenType::SUBTRACTION, TokenType::MULTIPLICATOR,TokenType::DIVISOR,		// , + - * /
				TokenType::AND, TokenType::OR, TokenType::NEGATIVE, TokenType::ASSIGNMENT,TokenType::GREATER_THAN,				// && || ! = >
				TokenType::GREATER_OR_EQUAL_TO, TokenType::LESSER_THAN, TokenType::LESSER_OR_EQUAL_TO, TokenType::EQUIVALENT,	// >= < <= ==
				TokenType::DIFFERENT, TokenType::MODULE																			// != % 
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}
		case Atom::TokenType::LITERAL:
		{
			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::LITERAL, TokenType::INT, TokenType::FLOAT, TokenType::CHAR,
				TokenType::IMPORT, TokenType::DEFINE, TokenType::OPEN_P, TokenType::OPEN_B, TokenType::OPEN_C_B, TokenType::SEMICOLON,
				TokenType::COMMA, TokenType::ASSIGNMENT, TokenType::DIFFERENT, TokenType::QUOTATION_MARKS, TokenType::RETURN,
				TokenType::ADDITION, TokenType::ENUM, TokenType::STRUCT, TokenType::CLOSE_C_B
			};
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::LITERAL, TokenType::OPEN_P, TokenType::OPEN_B, TokenType::OPEN_C_B, TokenType::SEMICOLON, TokenType::COMMA,
				TokenType::ASSIGNMENT, TokenType::DIFFERENT, TokenType::CLOSE_P, TokenType::QUOTATION_MARKS,
				TokenType::ADDITION, TokenType::SUBTRACTION, TokenType::CLOSE_C_B
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::VECTOR_CHAR:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::OPEN_P,TokenType::OPEN_C_B, TokenType::COMMA, TokenType::ASSIGNMENT, TokenType::RETURN };
			std::vector<TokenType> acceptableNextTokens{ TokenType::SEMICOLON, TokenType::COMMA, TokenType::CLOSE_P, TokenType::CLOSE_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::INT:
		case Atom::TokenType::FLOAT:
		case Atom::TokenType::CHAR:
		{
			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::OPEN_B, TokenType::CLOSE_B, TokenType::OPEN_C_B, TokenType::CLOSE_C_B,
				TokenType::OPEN_P, TokenType::CLOSE_P, TokenType::SEMICOLON, TokenType::FUNCTION, TokenType::COMMA
			};
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}
		case Atom::TokenType::WHILE:
		case Atom::TokenType::FOR:
		case Atom::TokenType::IF:
		case Atom::TokenType::ELSEIF:
		case Atom::TokenType::ELSE:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::OPEN_C_B, TokenType::CLOSE_C_B, TokenType::SEMICOLON };
			std::vector<TokenType> acceptableNextTokens{ TokenType::OPEN_P };


			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::BREAK:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::OPEN_C_B, TokenType::CLOSE_C_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::RETURN:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::SEMICOLON };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::INT, TokenType::FLOAT, TokenType::CHAR, TokenType::SEMICOLON	};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CONTINUE:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B, TokenType::OPEN_C_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::ENUM:
		case Atom::TokenType::STRUCT:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::OPEN_C_B, TokenType::CLOSE_C_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::OPEN_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::IMPORT:
		case Atom::TokenType::DEFINE:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::SEMICOLON };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::IMPORT };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::FUNCTION:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::SEMICOLON, TokenType::CLOSE_C_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::INT, TokenType::FLOAT, TokenType::CHAR, TokenType::LITERAL };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::AND:
		case Atom::TokenType::OR:
		case Atom::TokenType::NEGATIVE:
		case Atom::TokenType::ASSIGNMENT:
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
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::NUMBER };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::OPEN_C_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::OPEN_B:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::OPEN_B, TokenType::CLOSE_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::NUMBER, TokenType::LITERAL, TokenType::OPEN_B, TokenType::CLOSE_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CLOSE_B:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::OPEN_B, TokenType::CLOSE_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::EQUIVALENT, TokenType::ASSIGNMENT, TokenType::OPEN_B, TokenType::CLOSE_B };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::OPEN_C_B:
		case Atom::TokenType::CLOSE_C_B:
		{
			std::vector<TokenType> acceptablePreviousTokens
			{
				TokenType::LITERAL, TokenType::CLOSE_P, TokenType::SEMICOLON, TokenType::CLOSE_C_B, TokenType::OPEN_C_B,
				TokenType::CLOSE_C_B, TokenType::ASSIGNMENT
			};
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::LITERAL, TokenType::FOR, TokenType::WHILE, TokenType::IF, TokenType::ELSEIF, TokenType::ELSE,
				TokenType::INT, TokenType::FLOAT, TokenType::CHAR,
				TokenType::OPEN_C_B, TokenType::CLOSE_C_B, TokenType::QUOTATION_MARKS, TokenType::SEMICOLON
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::OPEN_P:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::CLOSE_P, TokenType::SEMICOLON };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::QUOTATION_MARKS, TokenType::APOSTROPHE, TokenType::INT, TokenType::FLOAT, TokenType::CHAR };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::CLOSE_P:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::CLOSE_P, TokenType::CLOSE_C_B, TokenType::SEMICOLON, TokenType::QUOTATION_MARKS, TokenType::APOSTROPHE };
			std::vector<TokenType> acceptableNextTokens{ TokenType::CLOSE_P, TokenType::OPEN_P, TokenType::OPEN_C_B, TokenType::OPEN_B, TokenType::SEMICOLON };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::SEMICOLON:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::CLOSE_B, TokenType::CLOSE_C_B, TokenType::CLOSE_P };
			std::vector<TokenType> acceptableNextTokens
			{
				TokenType::LITERAL, TokenType::INT, TokenType::FLOAT,TokenType::CHAR,
				TokenType::FOR, TokenType::WHILE, TokenType::IF, TokenType::ELSEIF, TokenType::ELSE,
				TokenType::SEMICOLON, TokenType::FUNCTION, TokenType::RETURN, TokenType::IMPORT, TokenType::ENUM,
				TokenType::CLOSE_C_B, TokenType::STRUCT
			};

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		case Atom::TokenType::COMMA:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::CLOSE_B, TokenType::CLOSE_P, TokenType::CLOSE_C_B };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::NUMBER, TokenType::INT, TokenType::FLOAT, TokenType::CHAR };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}
		case Atom::TokenType::APOSTROPHE:
		case Atom::TokenType::QUOTATION_MARKS:
		{
			std::vector<TokenType> acceptablePreviousTokens{ TokenType::LITERAL, TokenType::OPEN_C_B, TokenType::COMMA, TokenType::OPEN_P, TokenType::CHAR };
			std::vector<TokenType> acceptableNextTokens{ TokenType::LITERAL, TokenType::CHAR, TokenType::QUOTATION_MARKS, TokenType::SEMICOLON, TokenType::CLOSE_P };

			if (std::find(acceptableNextTokens.begin(), acceptableNextTokens.end(), next) != acceptableNextTokens.end()) calculateNext = true;
			if (std::find(acceptablePreviousTokens.begin(), acceptablePreviousTokens.end(), previous) != acceptablePreviousTokens.end()) calculatePrevious = true;

			break;
		}

		// unused because comments are handled by lexer
		case Atom::TokenType::COMMENT_L:	return true;
		case Atom::TokenType::COMMENTS_O:	return true;
		case Atom::TokenType::COMMENTS_C:	return true;
		case Atom::TokenType::TOKEN_MAX:	return false;
		default: return false;
		}

		return true;
	}
}