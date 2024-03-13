#pragma once
#include "Core/Common/Types.h"
#include <fstream>

namespace Nui::Log
{
	namespace Internal
	{
		/**
		* @brief Represents a log file
		*/
		class LogFile
		{
		public:
			LogFile() = default;

			/*
			* @brief Constructor for LogFile - Opens the log file
			* @param path The path to the log file
			*/
			LogFile(const fs::path& path);

			/*
			* @brief Destructor for LogFile - Closes the log file
			*/
			~LogFile();

			/*
			* @brief Writes a message to the log file
			* @param message The message to write
			*/
			void Write(const String& message);

			/*
			* @brief Flushes the log file buffer
			*/
			void Flush();

		private:
			/*
			* @brief The path to the log file
			*/
			String m_path;

			/*
			* @brief The log file stream
			*/
			std::ofstream m_file;
		};

		/**
		* @brief Opens the global log file
		* @param path The path to the log file
		*/
		void OpenGlobalLogFile(const fs::path& path);

		/**
		* @brief Closes the global log file
		* @note It's fine if this function is not called since the log file is automatically closed when the program exits
		*/
		void CloseGlobalLogFile();

	}  // namespace Internal

	/**
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

	/**
	*/
	String GetTimestamp(const chrono::system_clock::time_point& timePoint = chrono::system_clock::now());

	/**
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
}  // namespace Nui::Log

#define NUI_LOG(Level, Category, Message) Nui::Log::Log(Nui::Log::LogEntry(Nui::Log::LogLevel::Level, #Category, Message))

#if NUI_DEBUG
#define NUI_ASSERT(Condition, Message) Nui::Log::Assert(Condition, #Condition, Message, __FILE__, __LINE__)
#else
#define NUI_ASSERT(Condition, Message)
#endif