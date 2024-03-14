#include "Core/Engine/Engine.h"
#include <Core/Utils/Filesystem.h>

namespace Nui::Engine
{
	namespace  // anonymous namespace
	{
		static std::unique_ptr<AppBase> app{ nullptr };
	}  // anonymous namespace

	bool Init()
	{
		// Open global log file
		Log::Internal::OpenGlobalLogFile(Filesystem::GetCurrentWorkingDirectory() / "Saved" / "NuiEngine.log");

		// Make application
		app = Internal::MakeApp();


		return true;
	}

	void Update()
	{
		while (!app->WantsToClose())
		{

		}
	}

	void Shutdown()
	{
		app.reset();

		Log::Internal::CloseGlobalLogFile();
	}
}