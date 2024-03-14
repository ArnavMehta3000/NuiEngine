#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/AppBase.h"

namespace Nui::Engine
{
	namespace Internal
	{
		extern std::unique_ptr<Nui::AppBase> MakeApp();
	}

	bool Init();
	void Update();
	void Shutdown();
}