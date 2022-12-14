#pragma once
#include "Core/Core.h"
#include "Util/Memory.h"

#include <sstream>
#include <vector>

namespace Atom
{
    struct API Error
    {
        enum class Severity : unsigned int
        {
            NO_ERROR, WARNING, FATAL
        };

        enum class Phase : unsigned int
        {
            UNKNOWN, LEXER, PARSER, SEMANTIC
        };

        unsigned int Line = 1;
        unsigned int Column = 1;
        Severity Level = Severity::NO_ERROR;
        Phase From = Phase::UNKNOWN;
        std::string File = {};
        std::ostringstream Message = {};

        // returns a new smart pointer to the error itself
        static SharedRef<Error> Create(std::string file = nullptr, unsigned int line = 0, unsigned int column = 0, Severity level = Severity::FATAL, Phase from = Phase::UNKNOWN);

        // constructor
        Error(std::string file = nullptr, unsigned int line = 0, unsigned int column = 0, Severity level = Severity::FATAL, Phase from = Phase::UNKNOWN);

        // formats the information into a string
        std::string Format();
    };

    class API ErrorSystem
    {
    public:

        // returns a smart pointer to a new error system
        static SharedRef<ErrorSystem> Create();

        // constructor
        ErrorSystem();

        // destructor
        ~ErrorSystem();

        // returns the vector of errors
        inline std::vector<SharedRef<Error>>& GetErrors() { return m_Errors; }

        // returns the vector of feedbacks
        inline std::vector<SharedRef<Error>>& GetFeedbacks() { return m_Feedback; }

    public:

        // adds an error/warning to the error list
        inline void AddError(SharedRef<Error> e) { m_Errors.push_back(e); };

        // adds the feedback to the feedback list
        inline void AddFeedback(SharedRef<Error> e) { m_Feedback.push_back(e); };

        // clears errors/warnings and feedbacks
        inline void Clear() { m_Errors.clear(); m_Feedback.clear(); }

    private:

        std::vector<SharedRef<Error>> m_Errors;
        std::vector<SharedRef<Error>> m_Feedback;
    };
}