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
	private:
		std::unique_ptr<AppBase> m_app;
	};
}