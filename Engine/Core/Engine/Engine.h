#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/AppBase.h"

namespace Nui
{
	/**
	 * @brief Core engine class for Nui Engine
	 * @note This class is a singleton
	 */
	class Engine
	{
	public:
		/**
		 * @brief Default constructor that Initializes the engine
		 */
		Engine();

		/**
		 * @brief Destructor that shuts down the engine
		 */
		~Engine();

		/**
		 * @brief Runs the main loop of the engine
		 */
		void Run();

		/**
		 * @brief Quits the engine
		 */
		void Quit();

		/**
		 * @brief Returns the up time of the engine
		 * @return Up time in seconds
		 */
		inline F64 GetEngineUpTime() const noexcept { return m_engineTimer.GetElapsedSeconds(); }

		/**
		 * @brief Gets the application run by the engine
		 * @return Pointer to the application
		 */
		AppBase* GetApp() const noexcept;

	private:
		/**
		 * @brief Unique pointer to the application run by the engine
		 */
		std::unique_ptr<AppBase> m_app;

		/**
		 * @brief Whether or not the engine is running
		 */
		bool m_isRunning;

		/**
		 * @brief Up time of the engine
		 */
		Timer m_engineTimer;
	};
}