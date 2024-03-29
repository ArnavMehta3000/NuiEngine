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

		static Engine& Get();
		void Run();

	private:
		Engine();
		~Engine();
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;

	private:
		std::unique_ptr<AppBase> m_app;
	};
}