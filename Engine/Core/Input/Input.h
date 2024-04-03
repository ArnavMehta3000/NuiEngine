#pragma once
#include "Core/Common/CommonHeaders.h"
#include "InputDevice.h"

namespace Nui
{
	namespace Input
	{
		namespace Internal
		{
			[[nodiscard]] 
			bool ProcessInputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void Update();
		}

		[[nodiscard]] const Mouse::Point& GetMousePosition();
		[[nodiscard]] const Mouse::Point& GetMouseRawDelta();
		[[nodiscard]] const Mouse::WheelInfo& GetMouseWheelH();
		[[nodiscard]] const Mouse::WheelInfo& GetMouseWheelV();
		[[nodiscard]] const Mouse::ButtonState& GetMouseButton(Mouse::Button btn);
		[[nodiscard]] const Mouse::ButtonState& GetMouseButton(U32 btn);

		[[nodiscard]] const Keyboard::KeyState& GetKeyState(KeyCode key);
	}
}