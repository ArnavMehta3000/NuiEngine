#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Core/ECS/Universe.h"

namespace Nui
{
	Engine::Engine()
	{
		// Start engine up timer
		m_engineTimer.Start();

		Timer timer;
		timer.Start();

		// Open engine log file
		Log::Internal::OpenLogFile(Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		NUI_LOG(Debug, Engine, "Initializing Nui Engine...");

		auto world = ECS::World::Create<ECS::TestWorld>();
		auto world2 = ECS::World::Create<ECS::TestWorld2>();

		// Make application
		m_app = Internal::MakeApp();
		NUI_ASSERT(m_app.get(), "Failed to create application");

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine initialized successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
	}

	void Engine::Run()
	{
		Timer updateLoop;
		updateLoop.Start();

		F64 now = 0.0, dt = 0.0, elapsed = 0.0;
		while (!m_app->WantsToClose())
		{
			now = updateLoop.GetElapsedSeconds();

			Input::Internal::Update();

			// Update application

			dt = now - elapsed;
			elapsed = now;
		}

		updateLoop.Stop();
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