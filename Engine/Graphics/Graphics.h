#pragma once
#include "Graphics/Rendering/D3DManager.h"
#include "Graphics/Types/Types.h"
#include "Graphics/States/States.h"

namespace Nui::Graphics
{
	bool Init(HWND outputWindow);
	void Shutdown();
}