#include "Log.h"
#include "Core/Common/NuiWin.h"
#include "Core/Utils/Filesystem.h"
#include <format>
#include <stdexcept>

namespace Nui::Log
{
	namespace  // Anonymous namespace
	{
		// Global log file
		std::unique_ptr<Internal::LogFile> g_logFile{ nullptr };


		/*
		* @brief Log message to output window
		* @param message Message to log
		*/
		void LogAll(const String& message)
		{
			OutputDebugStringA(message.c_str());

			if (g_logFile)
			{
				g_logFile->Write(message);
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

			NUI_LOG(Debug, Log, "Opened log file: " + path.string());

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

		void OpenGlobalLogFile(const fs::path& path)
		{
			// Create parent directory if it doesn't exist
			if (!Filesystem::Exists(path.parent_path()))
			{
				Nui::Filesystem::MakeDirectory(path.parent_path());
				NUI_LOG(Debug, Log, "Created log directory: " + path.parent_path().string());
			}
			g_logFile = std::make_unique<LogFile>(path);
		}

		void CloseGlobalLogFile()
		{
			g_logFile.reset();
		}
	}  // namespace Internal


	LogEntry::LogEntry(LogLevel level, StringView category, StringView message, Nui::Stacktrace trace)
		: Level(level)
		, Category(category)
		, Message(message)
		, Stacktrace(trace)
		, Time(chrono::system_clock::now())
	{}

	void Log(const LogEntry& entry)
	{
		// TODO: Make logging multi-threaded		

		String formattedMsg1 = std::format("{} [{}] ", GetTimestamp(entry.Time), LogLevelToString(entry.Level));
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

	String GetTimestamp(const chrono::system_clock::time_point& timePoint)
	{
		auto time = chrono::system_clock::to_time_t(timePoint);
		auto ms = chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()) % 1000;
		auto msInt = static_cast<int>(ms.count());

		std::tm tm;
		localtime_s(&tm, &time);

		return std::format("[{:02d}.{:02d}.{:02d}.{:03d}]",
			tm.tm_hour, tm.tm_min, tm.tm_sec, msInt);
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

			throw std::runtime_error("Assertion failed");
		}
	}
}  // namespace Nui::Log