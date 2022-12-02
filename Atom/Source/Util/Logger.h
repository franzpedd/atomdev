#pragma once
#include "Core/Core.h"

#include <cstdint>
#include <cstdarg>
#include <cstdio>

#define LOG_MAX_SIZE 1024

namespace Atom
{
	class API Logger
	{
	public:

		typedef enum Severity
		{
			TRACE = 0,
			INFO,
			WARN,
			ERROR,
			ASSERTION,
		} Severity;

	public:

		// sends a message to the console
		static void ToConsole(Severity severity, const char* file, unsigned int line, const char* message, ...)
		{
			printf("[%s]", SeverityToConstChar(severity));

			char buffer[LOG_MAX_SIZE];
			va_list args;
			va_start(args, message);
			vsnprintf(buffer, LOG_MAX_SIZE, message, args);
			va_end(args);

			printf(" %s\n", buffer);
		}

		// returns the severity as text
		static const char* SeverityToConstChar(Severity severity)
		{
			switch (severity)
			{
				case Severity::TRACE: return "Trace";
				case Severity::INFO: return "Info";
				case Severity::WARN: return "Warn";
				case Severity::ERROR: return "Error";
				case Severity::ASSERTION: return "Assertion";
			}

			return "Unknown";
		}
	};
}

// always enable assertions, erros and warnings
#define LOGGER_WARN(...)	Atom::Logger::ToConsole(Atom::Logger::Severity::WARN, __FILE__, __LINE__, __VA_ARGS__);
#define LOGGER_ERROR(...)	Atom::Logger::ToConsole(Atom::Logger::Severity::ERROR, __FILE__, __LINE__, __VA_ARGS__);
#define ASSERT(x, ...) { if(!(x)) { Atom::Logger::ToConsole(Atom::Logger::Severity::ASSERTION, __FILE__, __LINE__, __VA_ARGS__); abort();}}

// enable tracing and info on debug version
#if defined ATOM_DEBUG || EDITOR_DEBUG
	#define LOGGER_TRACE(...)	Atom::Logger::ToConsole(Atom::Logger::Severity::TRACE, __FILE__, __LINE__, __VA_ARGS__);
	#define LOGGER_INFO(...)	Atom::Logger::ToConsole(Atom::Logger::Severity::INFO, __FILE__, __LINE__, __VA_ARGS__);
#else
	#define LOGGER_TRACE(...)
	#define LOGGER_INFO(...)
#endif