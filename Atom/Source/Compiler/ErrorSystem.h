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

        unsigned int Line;
        unsigned int Scope;
        Severity Level;
        std::ostringstream Message;

        // returns a new smart pointer to the error itself
        static SharedRef<Error> Create(unsigned int line = 0, unsigned int scope = 0, Severity level = Severity::FATAL);

        // constructor
        Error(unsigned int line = 0, unsigned int scope = 0, Severity level = Severity::FATAL);

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