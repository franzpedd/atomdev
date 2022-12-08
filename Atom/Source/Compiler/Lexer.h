#pragma once

#include <vector>
#include <fstream>

#include "Core/Core.h"
#include "ErrorSystem.h"
#include "Token.h"
#include "Util/Memory.h"

namespace Atom
{
	class API Lexer
	{
	public:

		// returns a pointer to a new lexer
		static SharedRef<Lexer> Create(SharedRef<ErrorSystem>& errorsystem);

		// constructor
		Lexer(SharedRef<ErrorSystem>& errorsystem);

		// destructor
		~Lexer();

	public:

		// returns a list of tokens
		inline std::vector<SharedRef<Token>>& GetTokens() { return m_Tokens; }

	public:

		// performs the lexical analyses
		void Lex(const char* file, bool print);
		
	private:

		// transforms the contents of a file to a list of tokens and saves it
		void Interpret(const char* file);

		// modifies variables of type string to it's real type and saves it
		void PostInterpret();

	private:

		SharedRef<ErrorSystem>& m_ErrorSystem;
		std::vector<SharedRef<Token>> m_Tokens;
	};
}