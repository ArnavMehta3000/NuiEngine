#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Core/Utils/Timer.h"

namespace Nui::Engine
{
	namespace  // anonymous namespace
	{
		static std::unique_ptr<AppBase> s_app{ nullptr };
		static bool s_initialized{ false };
	}  // anonymous namespace

	Nui::AppBase* const GetApp()
	{
		return s_app.get();
	}

	bool Init()
	{
		Timer timer;
		timer.Start();

		// Open engine log file
		Log::Internal::OpenLogFile(
			Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		NUI_LOG(Debug, Engine, "Initializing Nui Engine...");

		// Make application
		s_app = Internal::MakeApp();

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine initialized successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
		
		s_initialized = true;
		return s_initialized;
	}

	void MainLoop()
	{
		NUI_ASSERT(s_initialized, "Engine must be initialized before calling MainLoop");

		NUI_LOG(Debug, Engine, "Starting main loop...");

		while (!s_app->WantsToClose())
		{

		}
	}

	void Shutdown()
	{
		Timer timer;
		timer.Start();

		NUI_LOG(Debug, Engine, "Shutting down Nui Engine...");
		s_app.reset();

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine shut down successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
		Log::Internal::CloseLogFile();
	}
}