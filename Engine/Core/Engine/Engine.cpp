#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"

namespace Nui
{
	Engine::Engine()
		: m_isRunning(true)
	{
		// Start engine up timer
		m_engineTimer.Start();

		Timer timer;
		timer.Start();

		// Open engine log file
		Log::Internal::OpenLogFile(Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		NUI_LOG(Debug, Engine, "Initializing Nui Engine...");

		// Make application
		m_app = Internal::MakeApp();
		NUI_ASSERT(m_app.get(), "Failed to create application");

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine initialized successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
	}

	AppBase* Engine::GetApp() const noexcept
	{
		return m_app.get();
	}

	void Engine::Run()
	{
		m_app->OnInit();

		Timer updateLoop;
		updateLoop.Start();

		F64 now = 0.0, dt = 0.0, elapsed = 0.0;
		while (m_isRunning && !m_app->WantsToClose())
		{
			now = updateLoop.GetElapsedSeconds();

			Input::Internal::Update();
			// Update application
			m_app->Tick(dt);

			dt = now - elapsed;
			elapsed = now;
		}

		updateLoop.Stop();

		m_app->OnShutdown();
	}

	void Engine::Quit()
	{
		m_isRunning = false;
	}

	Engine::~Engine()
	{
		Timer timer;
		timer.Start();

		NUI_LOG(Debug, Engine, "Shutting down Nui Engine...");
		m_app.reset();

		timer.Stop();
		m_engineTimer.Stop();

		NUI_LOG(Debug, Engine, "Nui Engine shut down successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
		NUI_LOG(Debug, Engine, "Nui Engine was active for ", m_engineTimer.GetElapsedSeconds().ToString(), " seconds");

		Log::Internal::CloseLogFile();
	}
}