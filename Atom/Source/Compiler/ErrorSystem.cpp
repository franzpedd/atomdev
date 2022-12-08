#include "ErrorSystem.h"
#include "Util/Logger.h"

namespace Atom
{
    SharedRef<Error> Error::Create(unsigned int line, Severity level, Phase from)
    {
        return CreateSharedPointer<Error>(line, level, from);
    }

    Error::Error(unsigned int line, Severity level, Phase from)
        : Line(line), Level(level), From(from)
    {

    }

    std::string Error::Format()
    {
        std::ostringstream oss;

        if (Level == Severity::WARNING) oss << "[Warning]";
        else if (Level == Severity::FATAL) oss << "[Error]";
        else if(Level == Severity::NO_ERROR) oss << "[Feedback]";

        if (From == Phase::LEXER) oss << "[Lexer]";
        else if (From == Phase::PARSER) oss << "[Parser]";
        else if (From == Phase::SEMANTIC) oss << "[Semantic]";

        oss << "[Line " << Line << "] " << this->Message.str();

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