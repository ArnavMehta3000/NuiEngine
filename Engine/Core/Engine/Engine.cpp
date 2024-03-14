#include "Core/Engine/Engine.h"
#include <Core/Utils/Filesystem.h>

namespace Nui::Engine
{
	namespace  // anonymous namespace
	{
		static std::unique_ptr<AppBase> app{ nullptr };
		static bool s_initialized{ false };
	}  // anonymous namespace

	Nui::AppBase* const GetApp()
	{
		return app.get();
	}

	bool Init()
	{
		// Open engine log file
		Log::Internal::OpenLogFile(
			Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		// Make application
		app = Internal::MakeApp();

		s_initialized = true;
		return s_initialized;
	}

	void MainLoop()
	{
		NUI_ASSERT(s_initialized, "Engine must be initialized before calling MainLoop");

		while (!app->WantsToClose())
		{

		}
	}

	void Shutdown()
	{
		app.reset();

		Log::Internal::CloseLogFile();
	}
}