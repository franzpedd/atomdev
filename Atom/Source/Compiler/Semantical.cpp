#include "Semantical.h"
#include "Util/Logger.h"

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
		m_FunctionTable.clear();
		m_SymbolTable.clear();

		LOGGER_TRACE("Analyzing %s", file);

		std::string lastfunc = { "global" };
		m_SymbolTable[lastfunc] = { "global", TokenType::FUNCTION };
		m_FunctionTable[lastfunc] = { "global", TokenType::FUNCTION };
		m_FunctionTable["print"] = { "print", TokenType::FUNCTION };

		for (size_t i = 0; i < tokens.size(); i++)
		{
			// verifies if functions have the same name
			if (tokens[i]->GetSpecification().Type == TokenType::FUNCTION)
			{
				if (tokens[i + 1] != nullptr && tokens[i + 2] != nullptr)
				{
					SharedRef<Token> next = tokens[i + 1];
					SharedRef<Token> nextNext = tokens[i + 2];

					if (nextNext->GetSpecification().Type == TokenType::LITERAL &&
						(next->GetSpecification().Type == TokenType::INT
							|| next->GetSpecification().Type == TokenType::FLOAT
							|| next->GetSpecification().Type == TokenType::CHAR))
					{
						lastfunc = nextNext->GetSpecification().String;

						if (m_FunctionTable.find(lastfunc) != m_FunctionTable.end()) // exists
						{
							SharedRef<Error> e = Error::Create
							(
								tokens[i]->GetSpecification().File,
								tokens[i]->GetSpecification().Line,
								tokens[i]->GetSpecification().Column,
								Error::Severity::FATAL,
								Error::Phase::SEMANTIC
							);
							e->Message << "Redefinicao da funcao: " << lastfunc << std::endl;
							m_ErrorSystem->AddError(e);
						}

						else // doesnt exists
						{
							m_FunctionTable[nextNext->GetSpecification().String] = { nextNext->GetSpecification().String, TokenType::FUNCTION };
							SharedRef<Error> e = Error::Create
							(
								tokens[i]->GetSpecification().File,
								tokens[i]->GetSpecification().Line,
								tokens[i]->GetSpecification().Column,
								Error::Severity::NO_ERROR,
								Error::Phase::SEMANTIC
							);

							e->Message << "Definindo funcao: " << nextNext->GetSpecification().String << std::endl;
							m_ErrorSystem->AddFeedback(e);
						}
					}
				}
			}
		}

		// populate hashtable, defining variables
		for (size_t i = 0; i < tokens.size(); i++)
		{
			bool variable = false;
			variable |= tokens[i]->GetSpecification().Type == TokenType::INT;
			variable |= tokens[i]->GetSpecification().Type == TokenType::CHAR;
			variable |= tokens[i]->GetSpecification().Type == TokenType::FLOAT;

			if (variable && tokens[i + 1] != nullptr)
			{
				bool variable = tokens[i + 1]->GetSpecification().Type == TokenType::LITERAL;
				
				if (variable)
				{
					bool notOnSymbols = m_SymbolTable.find(tokens[i + 1]->GetSpecification().String) == m_SymbolTable.end();
					bool notOnFunctions = m_FunctionTable.find(tokens[i + 1]->GetSpecification().String) == m_FunctionTable.end();

					if (!notOnFunctions) continue;

					if (notOnSymbols)
					{
						m_SymbolTable[tokens[i+1]->GetSpecification().String] = {tokens[i+1]->GetSpecification().String, tokens[i+1]->GetSpecification().Type};
						
						SharedRef<Error> e = Error::Create
						(
							tokens[i]->GetSpecification().File,
							tokens[i]->GetSpecification().Line,
							tokens[i]->GetSpecification().Column,
							Error::Severity::NO_ERROR,
							Error::Phase::SEMANTIC
						);

						e->Message << "Definindo variavel: " << tokens[i + 1]->GetSpecification().String << std::endl;
						m_ErrorSystem->AddFeedback(e);
					}

					else
					{
						SharedRef<Error> e = Error::Create
						(
							tokens[i]->GetSpecification().File,
							tokens[i]->GetSpecification().Line,
							tokens[i]->GetSpecification().Column,
							Error::Severity::FATAL,
							Error::Phase::SEMANTIC
						);

						e->Message << "Redefinicao de variavel: " << tokens[i + 1]->GetSpecification().String << std::endl;
						m_ErrorSystem->AddError(e);
					}
				}
			}
		}

		// analyses if variable exists or not
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if(tokens[i]->GetSpecification().Type == TokenType::LITERAL)
			{
				bool exists = m_SymbolTable.find(tokens[i]->GetSpecification().String) != m_SymbolTable.end();
				exists |= m_FunctionTable.find(tokens[i]->GetSpecification().String) != m_FunctionTable.end();
				exists |= tokens[i - 1]->GetSpecification().Type == TokenType::IMPORT;

				std::string c = tokens[i]->GetSpecification().String;
				if (c[0] == '$' || c[0] == '&' || c[0] == '@' || c[0] == '^' || c[0] == '`' || c[0] == '~') continue;

				if (!exists)
				{
					SharedRef<Error> e = Error::Create
					(
						tokens[i]->GetSpecification().File,
						tokens[i]->GetSpecification().Line,
						tokens[i]->GetSpecification().Column,
						Error::Severity::FATAL,
						Error::Phase::SEMANTIC
					);

					e->Message << "Variavel " << tokens[i]->GetSpecification().String  << " nao existente" << std::endl;
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
						Error::Phase::SEMANTIC
					);

					e->Message << "Variavel " << tokens[i]->GetSpecification().String << " existente na tabela" << std::endl;
					m_ErrorSystem->AddFeedback(e);
				}
			}

			// accessing vector with floating number or anything else than a number
			if (tokens[i]->GetSpecification().Type == TokenType::OPEN_B && tokens[i + 1] != nullptr)
			{
				if (tokens[i + 1]->GetSpecification().Type != TokenType::NUMBER)
				{
					SharedRef<Error> e = Error::Create
					(
						tokens[i]->GetSpecification().File,
						tokens[i]->GetSpecification().Line,
						tokens[i]->GetSpecification().Column,
						Error::Severity::FATAL,
						Error::Phase::SEMANTIC
					);

					e->Message << "Acessando um valor nao inteiro: " << tokens[i]->GetSpecification().String << std::endl;
					m_ErrorSystem->AddError(e);
				}
			}

			// check if mathematical operatipon is made beetween same types
			bool mathematicalOperation = false;
			mathematicalOperation |= tokens[i]->GetSpecification().Type == TokenType::ADDITION;
			mathematicalOperation |= tokens[i]->GetSpecification().Type == TokenType::SUBTRACTION;
			mathematicalOperation |= tokens[i]->GetSpecification().Type == TokenType::MULTIPLICATOR;
			mathematicalOperation |= tokens[i]->GetSpecification().Type == TokenType::DIVISOR;
			mathematicalOperation |= tokens[i]->GetSpecification().Type == TokenType::MODULE;

			if (mathematicalOperation)
			{
				if (tokens[i - 2] != nullptr && tokens[i - 1] != nullptr && tokens[i + 1] != nullptr)
				{
					SharedRef<Token> previousPrevious = tokens[i - 2];
					SharedRef<Token> previous = tokens[i - 1];
					SharedRef<Token> next = tokens[i + 1];

					bool isValid = previous->GetSpecification().Type == next->GetSpecification().Type;
					isValid |= previous->GetSpecification().Type == TokenType::ASSIGNMENT;
					isValid |= next->GetSpecification().Type == TokenType::ASSIGNMENT;
					isValid |= previousPrevious->GetSpecification().Type == TokenType::INT && next->GetSpecification().Type == TokenType::NUMBER;
					isValid |= previousPrevious->GetSpecification().Type == TokenType::FLOAT && next->GetSpecification().Type == TokenType::FLOATING_NUMBER;

					if (!isValid)
					{
						SharedRef<Error> e = Error::Create
						(
							tokens[i]->GetSpecification().File,
							tokens[i]->GetSpecification().Line,
							tokens[i]->GetSpecification().Column,
							Error::Severity::FATAL,
							Error::Phase::SEMANTIC
						);

						e->Message << "Realizando uma operacao invalida: " 
							<< previous->GetSpecification().String << " "
							<< tokens[i]->GetSpecification().String << " "
							<< next->GetSpecification().String 
							<< std::endl;
						m_ErrorSystem->AddError(e);
					}
				}
			}
		}

		LOGGER_TRACE("Hahtable de variaveis:");
		for (auto const& pair : m_SymbolTable)
		{
			LOGGER_TRACE("S: %s:%s", pair.first.c_str(), pair.second.Name.c_str());
		}

		LOGGER_TRACE("Hahtable de funcoes:");
		for (auto const& pair : m_FunctionTable)
		{
			LOGGER_TRACE("F: %s:%s", pair.first.c_str(), pair.second.Name.c_str());
		}
	}
}