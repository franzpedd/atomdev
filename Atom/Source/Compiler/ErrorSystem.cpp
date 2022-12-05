#include "ErrorSystem.h"
#include "Util/Logger.h"

namespace Atom
{
    SharedRef<Error> Error::Create(unsigned int line, unsigned int column, Severity level)
    {
        return CreateSharedPointer<Error>(line, column, level);
    }

    Error::Error(unsigned int line, unsigned int column, Severity level)
        : Line(line), Column(column), Level(level)
    {

    }

    std::string Error::Format()
    {
        std::ostringstream oss;

        if (this->Level == Severity::WARNING) oss << "[Warning]";
        else if (this->Level == Severity::FATAL) oss << "[Error]";

        oss << "[Line " << this->Line;
        oss << ":Column " << this->Column;
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

    void ErrorSystem::Print()
    {
        for (size_t i = 0; i < this->m_Errors.size(); i++)
        {
            printf("%s\n", this->m_Errors[i]->Format().c_str());
        }
    }
}