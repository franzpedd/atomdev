#include "ErrorSystem.h"
#include "Util/Logger.h"

namespace Atom
{
    SharedRef<Error> Error::Create(std::string file, unsigned int line, unsigned int column, Severity level, Phase from)
    {
        return CreateSharedPointer<Error>(file, line, column, level, from);
    }

    Error::Error(std::string file, unsigned int line, unsigned int column, Severity level, Phase from)
        : File(file), Line(line), Column(column), Level(level), From(from)
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
        else if (From == Phase::SEMANTIC) oss << "[Semantical]";

        oss << "[File " << File << "]";
        oss << "[Line " << Line << "]";
        oss << "[Column " << Column << "]";
        oss << Message.str();

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