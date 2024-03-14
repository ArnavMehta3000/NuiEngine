#pragma once
#include "Core/Common/Types.h"

namespace Nui
{
	/**
	* @brief Timer class
	*/
	class Timer
	{
	private:
		/**
		* @brief Time value helper struct
		*/
		struct Time
		{
			/**
			* @brief Time value
			*/
			F64 Time;
			
			/**
			* @brief Convert time value to string
			*/
			String ToString()
			{
				return std::to_string(Time);
			}

			/**
			* @brief Implicit conversion to F64
			*/
			operator F64() { return Time; }
		};

	public:
		/**
		* @brief Start timer
		*/
		void Start()
		{
			m_startTime = chrono::high_resolution_clock::now();
			m_isRunning = true;
		}

		/**
		* @brief Stop timer
		*/
		void Stop()
		{
			m_endTime = chrono::high_resolution_clock::now();
			m_isRunning = false;
		}

		/**
		* @brief Reset timer
		*/
		void Reset()
		{
			m_startTime = chrono::high_resolution_clock::now();
			m_isRunning = false;
		}

		/**
		* @brief Check if timer is running
		* @return True if timer is running
		*/
		bool IsRunning() const
		{
			return m_isRunning;
		}

		/**
		* @brief Get elapsed time in seconds
		* @return Elapsed time in seconds
		* @see GetElapsedMilliseconds
		*/
		Time GetElapsedSeconds()
		{
			return Time{ GetElapsedMilliseconds().Time / 1000.0 };
		}

		/**
		* @brief Get elapsed time in milliseconds
		* @return Elapsed time in milliseconds
		* @see GetElapsedSeconds
		*/
		Time GetElapsedMilliseconds()
		{
			chrono::time_point<chrono::high_resolution_clock> endTime;

			endTime = m_isRunning ? chrono::high_resolution_clock::now() : m_endTime;


			return Time{ static_cast<F64>(chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count()) };
		}

	private:
		chrono::time_point<chrono::high_resolution_clock> m_startTime;
		chrono::time_point<chrono::high_resolution_clock> m_endTime;
		bool m_isRunning = false;
	};
}