#include "ErrorSystem.h"
#include "Util/Logger.h"

namespace Atom
{
    SharedRef<Error> Error::Create(unsigned int line, unsigned int scope, Severity level)
    {
        return CreateSharedPointer<Error>(line, scope, level);
    }

    Error::Error(unsigned int line, unsigned int scope, Severity level)
        : Line(line), Scope(scope), Level(level)
    {

    }

    std::string Error::Format()
    {
        std::ostringstream oss;

        if (this->Level == Severity::WARNING) oss << "[Warning]";
        else if (this->Level == Severity::FATAL) oss << "[Error]";
        else if(this->Level == Severity::NO_ERROR) oss << "[Feedback]";

        oss << "[Line " << this->Line;
        oss << ":Scope " << this->Scope;
        oss << "] " << this->Message.str();

        return oss.str();
    }

    SharedRef<ErrorSystem> ErrorSystem::Create()
    {
        return CreateSharedPointer<ErrorSystem>();
    }

    ErrorSystem::ErrorSystem()
    {
        LOGGER_TRACE("Creating ErrorSystem");
    }

    ErrorSystem::~ErrorSystem()
    {

    }
}