#pragma once

#include "Core/Core.h"
#include "ErrorSystem.h"
#include "Token.h"
#include "Util/Memory.h"

namespace Atom
{
	class Semantical
	{
	public:

		// returns a smart pointer to a new semantical
		static SharedRef<Semantical> Create(SharedRef<ErrorSystem>& errorsystem);

		// constructor
		Semantical(SharedRef<ErrorSystem>& errorsystem);

		// destructor
		~Semantical();

	public:

		// performs semantical analises given a vector of tokens, at this point they must have been lexed/parsed
		void Analyze(const char* file, std::vector<SharedRef<Token>> tokens);

	private:

		SharedRef<ErrorSystem>& m_ErrorSystem;
	};
}