#pragma once

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX 1
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Windows.h>
#include <Core/Utils/Filesystem.h>
#include <Core/App/AppBase.h>

namespace Nui::Internal
{
	extern std::unique_ptr<Nui::AppBase> MakeApp();
}

#define NUI_DECLARE_APP(app)                              \
std::unique_ptr<Nui::AppBase> Nui::Internal::MakeApp()    \
{                                                         \
	return std::make_unique<app>();                       \
}


// Write the windows main function
int WINAPI wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR lpCmdLine,
	_In_     int nShowCmd
)
{
	std::unique_ptr<Nui::AppBase> app{ nullptr };

	try
	{
		Nui::Log::Internal::OpenGlobalLogFile(Nui::Filesystem::GetCurrentWorkingDirectory() / "Saved" / "Nui.log");
		app = Nui::Internal::MakeApp();
		while (!app->WantsToClose())
		{
			
		}
	}
	catch (const std::exception& e)
	{
		app.reset();
		NUI_LOG(Exception, Main, e.what());
	}

	app.reset();
	Nui::Log::Internal::CloseGlobalLogFile();

	return 0;
}