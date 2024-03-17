#pragma once
#include "Core/Common/Types.h"

namespace Nui
{
	/**
	* @brief Timestamp utility class, wraps chrono::system_clock::time_point
	*/
	class Timestamp
	{
	public:
		using ClockType = chrono::system_clock;
		using TimePoint = ClockType::time_point;

		/**
		* @brief Default constructor, sets timestamp to current time
		*/
		Timestamp()
			: m_point(ClockType::now())
		{}

		/**
		* @brief Constructor, sets timestamp to specified time
		*/
		Timestamp(const TimePoint& point)
			: m_point(point)
		{}

		/**
		* @brief Get formatted date and time (Format: "YYYY-MM-DD HH:MM:SS`")
		* @returns Formatted date and time
		*/
		String GetDateAndTime() const
		{
			const auto time = chrono::current_zone()->to_local(m_point);
			return std::format("{:%Y-%m-%d %X}", time);
		}

		/**
		* @brief Get formatted date (Format: "YYYY-MM-DD")
		* @returns Formatted date
		*/
		String GetDate() const
		{
			const auto time = chrono::current_zone()->to_local(m_point);
			return std::format("{:%Y-%m-%d}", time);
		}

		/**
		* @brief Get formatted time (Format: "HH:MM:SS")
		* @returns Formatted time
		*/
		String GetTime() const
		{
			const auto time = chrono::current_zone()->to_local(m_point);
			return std::format("{:%X}", time);
		}

		/**
		* @brief Implicitly convert timestamp to time point
		* @returns Time point
		*/
		inline operator TimePoint() const
		{
			return m_point;
		}

	private:
		TimePoint m_point;
	};
}