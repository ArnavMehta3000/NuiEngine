#pragma once

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX 1
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Windows.h>
#include <Core/Engine/Engine.h>



#define NUI_DECLARE_APP(app)                                  \
std::unique_ptr<Nui::AppBase> Nui::Engine::Internal::MakeApp()\
{                                                             \
	return std::make_unique<app>();                           \
}


// Write the windows main function
int WINAPI wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR lpCmdLine,
	_In_     int nShowCmd
)
{

	try
	{
		NUI_ASSERT(Nui::Engine::Init(), "Failed to initialize engine");
		Nui::Engine::Update();
		Nui::Engine::Shutdown();		
	}
	catch (const std::exception& e)
	{
		NUI_LOG(Exception, Main, e.what());
	}

	return 0;
}