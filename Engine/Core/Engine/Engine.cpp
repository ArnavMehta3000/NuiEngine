#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Core/ECS/World.h"

namespace Nui
{
	struct TestWorld : public ECS::World
	{
		TestWorld(std::string name)
		{
			NUI_LOG(Debug, TestWorld, "Hello World, ", name, "!");
		}
	};

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

		auto testWorld = m_app->GetUniverse()->CreateWorld<TestWorld>("Test World");

		timer.Stop();
		NUI_LOG(Debug, Engine, "Nui Engine initialized successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
	}

	AppBase* Engine::GetApp() const noexcept
	{
		return m_app.get();
	}

	void Engine::Run()
	{
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