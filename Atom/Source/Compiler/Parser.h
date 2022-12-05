#pragma once

#include <algorithm>

#include "Core/Core.h"
#include "ErrorSystem.h"
#include "Token.h"
#include "Util/Memory.h"

namespace Atom
{
	class API Parser
	{
	public:

		// returns a smart pointer to a new parser
		static SharedRef<Parser> Create(SharedRef<ErrorSystem>& errorsystem);

		// constructor
		Parser(SharedRef<ErrorSystem>& errorsystem);

		// destructor
		~Parser();

	public:

		// parses the token vector
		void Parse(const char* file, std::vector<SharedRef<Token>> tokens);

		// analyzes if a sequence of tokens is valid
		bool IsSequenceValid(TokenType current, TokenType previous, TokenType next);

	private:

		SharedRef<ErrorSystem>& m_ErrorSystem;
	};
}