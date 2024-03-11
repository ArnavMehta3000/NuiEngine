#pragma once

#pragma warning(push)
#pragma warning(disable : 4005)
#define NOMINMAX 1
#define NOMCX
#define NOSERVICE
#define NOHELP
#pragma warning(pop)

#include <Windows.h>
#include <Core/Common/CommonHeaders.h>

// TODO: Add command line arguments to be passed to the engine
extern int NuiMain();

#define NUI_MAIN() int NuiMain()

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
		return NuiMain();
	}
	catch (const std::exception& e)
	{
		NUI_LOG(Exception, Main, e.what());
	}

	return 0;
}