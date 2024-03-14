#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/AppBase.h"

namespace Nui::Engine
{
	namespace Internal
	{
		extern std::unique_ptr<Nui::AppBase> MakeApp();
	}

	Nui::AppBase* const GetApp();
	bool IsAppInitialized();

	bool Init();
	void MainLoop();
	void Shutdown();
}