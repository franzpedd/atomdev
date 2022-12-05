#pragma once

#include <vector>

#include "Core/Core.h"
#include "Util/Memory.h"

#include "Lexer.h"
#include "Parser.h"
#include "ErrorSystem.h"

/*
	Livro de regras:
	Primeiro token precisa ser import
	Ultimo token precisa ser }
	Variaveis nao podem ter numero

*/

namespace Atom
{
	class API Compiler
	{
	public:

		// returns a smart pointer to a new compiler
		static SharedRef<Compiler> Create();

		// constructor
		Compiler();

		// destructor
		~Compiler();

		// returns the list of project files
		inline std::vector<const char*>& GetProjectFiles() { return m_ProjectFiles; }

		// returns the error sytstem
		inline SharedRef<ErrorSystem>& GetErrorSystem() { return m_ErrorSystem; }

		// sets a new list of project files
		inline void SetProjectFiles(std::vector<const char*> projectfiles) { m_ProjectFiles = projectfiles; }

	public:

		// compiles all the list of files
		void CompileProject();

		// compile a single file
		void CompileFile(const char* path);

	private:

		SharedRef<ErrorSystem> m_ErrorSystem;
		SharedRef<Lexer> m_Lexer;
		SharedRef<Parser> m_Parser;
		std::vector<const char*> m_ProjectFiles;
	};
}