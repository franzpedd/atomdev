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

		// lexes a file
		void Lex(const char* file, bool print);

	private:

		SharedRef<ErrorSystem>& m_ErrorSystem;
		std::vector<SharedRef<Token>> m_Tokens;
	};
}