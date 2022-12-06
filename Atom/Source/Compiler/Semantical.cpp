#include "Semantical.h"
#include "Util/Logger.h"
#include "Util/String.h"

namespace Atom
{
	SharedRef<Semantical> Semantical::Create(SharedRef<ErrorSystem>& errorsystem)	
	{
		return CreateSharedPointer<Semantical>(errorsystem);
	}

	Semantical::Semantical(SharedRef<ErrorSystem>& errorsystem)
		: m_ErrorSystem(errorsystem)
	{
		LOGGER_TRACE("Creating Semantical");

	}

	Semantical::~Semantical()
	{

	}

	void Semantical::Analyze(const char* file, std::vector<SharedRef<Token>> tokens)
	{
		// analyze STRING types and see if they are actually other stuff
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (tokens[i]->GetSpecification().Type == TokenType::STRING && IsNumber(tokens[i]->GetSpecification().String))
			{
				TokenSpecification specs = tokens[i]->GetSpecification();
				std::string str = tokens[i]->GetSpecification().String;

				SharedRef<Token>& previousTypeDeclared = tokens[i - 2];

				if (previousTypeDeclared != nullptr)
				{
					specs.Type = previousTypeDeclared->GetSpecification().Type;
				}

				tokens[i]->SetSpecification(specs);
			}
		}

		// this passes trough all tokens and modifies the tokens that have a type of STRING
		// but have a variable declaration type just before then
		// it assumes there's no conflict with variable names but since we do not have
		// a tree of variables, we cannot assure
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (tokens[i]->GetSpecification().Type == TokenType::STRING)
			{
				SharedRef<Token>& previous = tokens[i - 1];

				bool previousIsVar = false;
				previousIsVar |= previous->GetSpecification().Type == TokenType::I_32;
				previousIsVar |= previous->GetSpecification().Type == TokenType::I_64;
				previousIsVar |= previous->GetSpecification().Type == TokenType::U_32;
				previousIsVar |= previous->GetSpecification().Type == TokenType::U_64;
				previousIsVar |= previous->GetSpecification().Type == TokenType::F_32;
				previousIsVar |= previous->GetSpecification().Type == TokenType::F_64;
				previousIsVar |= previous->GetSpecification().Type == TokenType::CHAR;

				if (previousIsVar)
				{
					TokenSpecification specs = tokens[i]->GetSpecification();
					specs.Type = previous->GetSpecification().Type;
					tokens[i]->SetSpecification(specs);
				}
			}
		}

		// this verifies if assignment can be made, it however assumes the lexer is correctly parsed
		// and that is not the case yet, first we need to pass trough the tokens, modifing its string values
		// to a variable type
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (tokens[i]->GetSpecification().Type == TokenType::ASSIGNMENT)
			{
				SharedRef<Token>& previous = tokens[i - 1];
				SharedRef<Token>& next = tokens[i + 1];

				bool canAssign = true;
				canAssign &= previous != nullptr && next != nullptr;
				canAssign &= previous->GetSpecification().Type == next->GetSpecification().Type;

				if (!canAssign)
				{
					SharedRef<Error> e = Error::Create(0, 0, Error::Severity::FATAL);
					e->Line = tokens[i]->GetSpecification().Line;
					e->Scope = tokens[i]->GetSpecification().Scope;
					e->Level = Error::Severity::FATAL;
					e->Message << "Impossible assignment " << TokenToString(previous->GetSpecification().Type);
					e->Message << " to " << TokenToString(next->GetSpecification().Type) << std::endl;
					m_ErrorSystem->AddError(e);
				}
			}
		}
	}
}