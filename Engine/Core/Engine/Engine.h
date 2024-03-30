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

		inline F64 GetEngineUpTime() const noexcept { return m_engineTimer.GetElapsedSeconds(); }

	private:
		std::unique_ptr<AppBase> m_app;
		Timer m_engineTimer;
	};
}