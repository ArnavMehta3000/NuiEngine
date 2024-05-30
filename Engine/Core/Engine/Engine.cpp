#include "Core/Engine/Engine.h"
#include "Core/Utils/Filesystem.h"
#include "Graphics/Graphics.h"
#include <thread>
#include <chrono>

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
		// Initialize graphics
		Graphics::Init(m_app->GetHWND());

		// Add resize callback for the graphics back buffers
		m_app->AddCallback(WM_SIZE, [&](NUI_WNDPROC_NAMED_ARGS) -> LRESULT
		{
			if (wParam == SIZE_MINIMIZED)
			{
				return 0;
			}

			U32 width  = LOWORD(lParam);
			U32 height = HIWORD(lParam);

			NUI_LOG(Debug, Engine->Graphics, "Back buffer resized to ", width, "x", height);
			
			Graphics::Resize(width, height);
			return 0;
		});

		// Initialize application
		m_app->PreInit();
		m_app->OnInit();

		Timer updateLoopTimer;
		updateLoopTimer.Start();

		// Main loop
		F64 now = 0.0, dt = 0.0, elapsed = 0.0;
		while (m_isRunning && !m_app->WantsToClose())
		{
			now = updateLoopTimer.GetElapsedSeconds();

			// Update input system
			Input::Internal::Update();

			// Update application
			m_app->Tick(dt);

			// Update delta time
			dt = now - elapsed;
			elapsed = now;

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(5ms);
		}

		updateLoopTimer.Stop();

		// Shutdown application
		m_app->OnShutdown();
		m_app->PostShutdown();

		// Shutdown graphics
		Graphics::Shutdown();
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