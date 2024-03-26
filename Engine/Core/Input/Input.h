#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/Input/InputEvent.h"

namespace Nui::Input
{
	namespace Internal
	{
		void Init();

		[[nodiscard]] bool ProcessInputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}