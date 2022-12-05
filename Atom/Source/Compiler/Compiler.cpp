#include "Compiler.h"
#include "Util/Logger.h"

namespace Atom
{
	SharedRef<Compiler> Compiler::Create()
	{
		return CreateSharedPointer<Compiler>();
	}

	Compiler::Compiler()
	{
		LOGGER_TRACE("Creating Compiler");

		m_ErrorSystem = ErrorSystem::Create();
		m_Lexer = Lexer::Create(m_ErrorSystem);
		m_Parser = Parser::Create(m_ErrorSystem);
	}

	Compiler::~Compiler()
	{

	}

	void Compiler::CompileProject()
	{

	}

	void Compiler::CompileFile(const char* path)
	{
		m_Lexer->Lex(path, true);
		m_Parser->Parse(path, m_Lexer->GetTokens());
		// m_Semantics->Analise(path);
	}
}