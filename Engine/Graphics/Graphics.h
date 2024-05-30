#pragma once
#include "Graphics/Rendering/D3DManager.h"
#include "Graphics/Types/Types.h"
#include "Graphics/States/States.h"

namespace Nui::Graphics
{
	void Init(HWND outputWindow);
	void Resize(U32 width, U32 height);
	void Shutdown();

	bool IsInitialized();
}