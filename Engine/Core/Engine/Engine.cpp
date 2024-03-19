#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Core/App/Input/Input.h"
#include <thread>

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
		Log::Internal::OpenLogFile(Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		NUI_LOG(Debug, Engine, "Initializing Nui Engine...");


		// Make application
		s_app = Internal::MakeApp();

		NUI_ASSERT(s_app.get(), "Failed to create application");

		// Initialize input
		Input::Internal::Init(s_app->GetHWND());

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine initialized successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
		
		s_initialized = true;
		return s_initialized;
	}

	void MainLoop()
	{
		NUI_ASSERT(s_initialized, "Engine must be initialized before calling MainLoop");

		NUI_LOG(Debug, Engine, "Starting main loop...");

		Timer updateLoop;
		updateLoop.Start();

		F64 now = 0.0, dt = 0.0, elapsed = 0.0;
		while (!s_app->WantsToClose())
		{
			now = updateLoop.GetElapsedSeconds();

			Input::Internal::PreUpdate(s_app->Focused());

			// Tick engine here

			Input::Internal::PostUpdate();

			dt = now - elapsed;
			elapsed = now;
		}
		updateLoop.Stop();
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