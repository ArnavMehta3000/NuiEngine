#pragma once
#include "Core/Common/Types.h"

namespace Nui::Log
{
	/*
    * @brief Log verbosity levels
    * @see LogEntry
    */
	enum class LogLevel
	{
		Debug = 0,   // For low level engine information
		Info,        // For general information
		Warn,        // For warnings
		Error,       // For errors
		Fatal,       // Similar to errors, but gives a stack trace
		Exception,   // Similar to errors, but gives a stack trace and crashes the program
	};

    /**
 * @class LogEntry
 * @brief Represents a log entry with its level, category, message, time, and stack trace.
 * @see LogLevel
 */
    struct LogEntry 
    {
        /**
         * @brief Constructor for LogEntry
         * @param level The log level
         * @param category The log category
         * @param message The log message
         * @param trace The stack trace
         * @see LogLevel
         */
        LogEntry(LogLevel level, StringView category, StringView message, Nui::Stacktrace trace = Nui::Stacktrace::current());

        /**
         * @brief The log level
         */
        LogLevel Level;

        /**
         * @brief The log category
         */
        String Category;

        /**
         * @brief The log message
         */
        String Message;

        /**
         * @brief The timestamp of the log entry
         */
        chrono::system_clock::time_point Time;

        /**
         * @brief The stack trace associated with the log entry
         */
        Stacktrace Stacktrace;
    };

    /**
    * @brief Logs a log entry to the VS output window.
    * @param entry The log entry
    * @see LogEntry
    */
	void Log(const LogEntry& entry);

	/*
    * @brief Asserts a condition and logs an error if it is not met.
    * @param condition The condition to assert
    * @param conditionString The string representation of the condition
    * @param message The error message
    * @param file The source file
    * @param line The source line
    * @param trace The stack trace
    * @throws std::runtime_error if the condition is not met
    */
	void Assert(bool condition, StringView conditionString, StringView message, StringView file, I32 line, Nui::Stacktrace trace = Nui::Stacktrace::current());
}

#define NUI_LOG(Level, Category, Message) Nui::Log::Log(Nui::Log::LogEntry(Nui::Log::LogLevel::Level, #Category, Message))

#if NUI_DEBUG
#define NUI_ASSERT(Condition, Message) Nui::Log::Assert(Condition, #Condition, Message, __FILE__, __LINE__)
#else
#define NUI_ASSERT(Condition, Message)
#endif