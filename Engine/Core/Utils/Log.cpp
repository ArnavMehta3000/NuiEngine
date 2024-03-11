#include "Log.h"
#include "Core/Common/NuiWin.h"
#include <format>
#include <stdexcept>

namespace Nui::Log
{
	namespace  // Anonymous namespace
	{
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
			OutputDebugStringA("\n ----- Begin Stack trace -----\n");
			for (auto& trace : stacktrace)
			{
				OutputDebugStringA((trace.description() + "\n").c_str());
				OutputDebugStringA(std::format("{}({})\n", 
					trace.source_file(), trace.source_line()).c_str());
				OutputDebugStringA("------------------------------------\n");
			}
			OutputDebugStringA("\n ----- End Stack trace -----\n");
		}
	}  // Anonymous namespace

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
		// TODO: Add log file

		auto time  = std::chrono::system_clock::to_time_t(entry.Time);
		auto ms    = std::chrono::duration_cast<std::chrono::milliseconds>(entry.Time.time_since_epoch()) % 1000;
		auto msInt = static_cast<int>(ms.count());

		std::tm tm;
		localtime_s(&tm, &time);

		String formattedTime = std::format("[{:02d}.{:02d}.{:02d}.{:03d}]",
			tm.tm_hour, tm.tm_min, tm.tm_sec, msInt);

		String formattedMsg1 = std::format("{} [{}] ", formattedTime, LogLevelToString(entry.Level));
		if (entry.Level == LogLevel::Info || entry.Level == LogLevel::Warn)
		{
			formattedMsg1 += " ";
		}
		String formattedMsg2 = std::format("[{}] - {}\n", entry.Category, entry.Message);

		OutputDebugStringA((formattedMsg1 + formattedMsg2).c_str());

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

			throw std::runtime_error("Assertion failed");
		}
	}
}