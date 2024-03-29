#pragma once
#include "Core/Common/CommonHeaders.h"
#include "InputDevice.h"

namespace Nui
{
	class Window;

	namespace Input
	{
		namespace Internal
		{
			[[nodiscard]] 
			bool ProcessInputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void Update();
		}

		const [[nodiscard]] Mouse::Point& GetMousePosition();
		const [[nodiscard]] Mouse::Point& GetMouseRawDelta();
		const [[nodiscard]] Mouse::WheelInfo& GetMouseWheelH();
		const [[nodiscard]] Mouse::WheelInfo& GetMouseWheelV();
		const [[nodiscard]] Mouse::ButtonState& GetMouseButton(Mouse::Button btn);
		const [[nodiscard]] Mouse::ButtonState& GetMouseButton(U32 btn);

		const [[nodiscard]] Keyboard::KeyState& GetKeyState(KeyCode key);
	}
}