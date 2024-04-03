#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/AppBase.h"

namespace Nui
{
	namespace Internal
	{
		extern std::unique_ptr<Nui::AppBase> MakeApp();
	}

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Run();
		void Quit();

		inline F64 GetEngineUpTime() const noexcept { return m_engineTimer.GetElapsedSeconds(); }
		AppBase* GetApp() const noexcept;

	private:
		std::unique_ptr<AppBase> m_app;
		bool m_isRunning;
		Timer m_engineTimer;
	};
}