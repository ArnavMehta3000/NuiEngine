#pragma once

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Core/Engine/Engine.h>
#include <Core/Utils/Singleton.h>
#include <Core/Engine/ECS/Component.h>

int WINAPI wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR lpCmdLine,
	_In_     int nShowCmd
)
{
	try
	{
		Nui::Singleton<Nui::Engine>::Get().Run();
		Nui::Singleton<Nui::Engine>::Destroy();
	}
	catch (const std::exception& e)
	{
		NUI_LOG(Exception, Main, e.what());
	}

	return 0;
}