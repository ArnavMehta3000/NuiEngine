#include "Log.h"
#include "Core/Common/NuiWin.h"
#include "Core/Utils/Filesystem.h"
#include <stdexcept>

namespace Nui::Log
{
	namespace  // Anonymous namespace
	{
		/**
		* @brief Global log file
		*/
		static std::unique_ptr<Internal::LogFile> s_logFile{ nullptr };


		/*
		* @brief Log message to output window
		* @param message Message to log
		*/
		void LogAll(const String& message)
		{
			OutputDebugStringA(message.c_str());

			if (s_logFile)
			{
				s_logFile->Write(message);
				s_logFile->Flush();
			}
		}

		/*
		* @brief Convert log level to string
		* @param level Log level
		* @return String representation of log level enum class
		*/
		StringView LogLevelToString(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Debug:
				return "Debug";
			case LogLevel::Info:
				return "Info";
			case LogLevel::Warn:
				return "Warn";
			case LogLevel::Error:
				return "Error";
			case LogLevel::Fatal:
				return "Fatal";
			case LogLevel::Exception:
				return "Exception";
			default:
				return "Undefined";
			}
		}

		/*
		* @brief Print stack trace
		* @param stacktrace Stack trace
		* @return None
		*/
		void PrintStackTrace(const Nui::Stacktrace& stacktrace)
		{
			LogAll("\n ----- Begin Stack trace -----\n");
			for (auto& trace : stacktrace)
			{
				LogAll((trace.description() + "\n"));
				LogAll(std::format("{}({})\n",
					trace.source_file(), trace.source_line()));
				LogAll("------------------------------------\n");
			}
			LogAll("\n ----- End Stack trace -----\n");
		}
	}  // Anonymous namespace


	namespace Internal
	{
		LogFile::LogFile(const fs::path& path)
			: m_path(path.string())
		{
			// std::trunc to override log file if it already exists
			m_file.open(path, std::ios::out | std::ios::trunc);

			NUI_LOG(Debug, Log, "Opened log file: ", path.string());

			NUI_ASSERT(m_file.is_open(), "Failed to open log file!");
		}

		LogFile::~LogFile()
		{
			if (m_file.is_open())
			{
				m_file.flush();
				m_file.close();
			}
		}

		void LogFile::Write(const String& message)
		{
			if (m_file.is_open())
			{
				m_file << message;
			}
			else
			{
				NUI_ASSERT(false, "Attempted to log to closed file!");
			}
		}

		void LogFile::Flush()
		{
			if (m_file.is_open())
			{
				m_file.flush();
			}
			else
			{
				NUI_ASSERT(false, "Attempted to flush closed file!");
			}
		}

		void OpenLogFile(const fs::path& path)
		{
			// Create parent directory if it doesn't exist
			if (!Filesystem::Exists(path.parent_path()))
			{
				Nui::Filesystem::MakeDirectory(path.parent_path());
				NUI_LOG(Debug, Log, "Created log directory: ", path.parent_path().string());
			}
			s_logFile = std::make_unique<LogFile>(path);
		}

		void CloseLogFile()
		{
			s_logFile.reset();
		}
	}  // namespace Internal


	LogEntry::LogEntry(LogLevel level, StringView category, StringView message, Nui::Stacktrace trace)
		: Level(level)
		, Category(category)
		, Message(message)
		, Stacktrace(trace)
		, Timestamp()
	{}

	void Log(const LogEntry& entry)
	{
		// TODO: Make logging multi-threaded	
		// TODO: Log based on config or allow user to set print verbosity
		String formattedMsg1 = std::format("[{}] [{}] ", entry.Timestamp.GetDateAndTime(), LogLevelToString(entry.Level));
		if (entry.Level == LogLevel::Info || entry.Level == LogLevel::Warn)
		{
			formattedMsg1 += " ";
		}
		String formattedMsg2 = std::format("[{}] - {}\n", entry.Category, entry.Message);

		LogAll(formattedMsg1 + formattedMsg2);

		// Only print stack trace on fatal errors
		if (entry.Level == LogLevel::Fatal)
		{
			PrintStackTrace(entry.Stacktrace);
		}
	}

	void Assert(bool condition, StringView conditionString, StringView message, StringView file, I32 line, Nui::Stacktrace trace)
	{
		if (!condition)
		{
			// Build log message using condition string and message
			String logMessage = std::format("Assertion Failed\n{}({})\n\tCondition: {}\n\tMessage: {}",
				file, line, conditionString, message);
			
			// Log with stack trace
			Log({ LogLevel::Exception, "Assert", logMessage, trace });
			PrintStackTrace(trace);
			s_logFile->Flush();

			throw std::runtime_error("Assertion failed");
		}
	}
}  // namespace Nui::Log