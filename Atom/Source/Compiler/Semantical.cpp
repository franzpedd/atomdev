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

	}
}