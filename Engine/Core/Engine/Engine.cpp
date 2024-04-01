#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Core/ECS/World.h"

namespace Nui
{
	Engine& Engine::Get()
	{
		static Engine s_instance;
		return s_instance;
	}

	Engine::Engine()
	{
		Timer timer;
		timer.Start();

		// Open engine log file
		Log::Internal::OpenLogFile(Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		NUI_LOG(Debug, Engine, "Initializing Nui Engine...");

		struct Test {int x; int y; };
		struct Test1 { int x; int y; };
		struct Test2 {int x; int y; };

		auto world = ECS::World();
		auto e1 = world.CreateEntity();
		auto e2 = world.CreateEntity();
		auto e3 = world.CreateEntity();
		world.AddComponent<Test>(e1);
		world.AddComponent<Test1>(e1);
		world.AddComponent<Test1>(e2);
		world.AddComponent<Test1>(e3);
		world.AddComponent<Test2>(e1);

		world.RemoveComponent<Test>(e1);

		auto c2 = world.GetComponent<Test2>(e1);
		c2->x = 5;
		auto c3 = world.GetComponent<Test2>(e1);
		c3->x = 10;

		ECS::WorldView<Test1> view(world);

		for (const auto& e : view)
		{
			auto id = e;

		}
		

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
		NUI_LOG(Debug, Engine, "Nui Engine shut down successfully in ", timer.GetElapsedSeconds().ToString(), " seconds");
		Log::Internal::CloseLogFile();
	}
}