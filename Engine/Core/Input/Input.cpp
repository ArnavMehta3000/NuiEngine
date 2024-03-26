#include "Input.h"

namespace Nui::Input
{
	namespace  // Anonymous namespace
	{
		static bool s_initialized = false;
		static std::queue<Internal::InputEvent> s_inputEvents;

		static bool IsVKDown(I32 vk)
		{
			return (::GetKeyState(vk) & 0x8000) != 0;
		}

		static Modifier GetModifiers()
		{
			Modifier m = Modifier::None;
			
		}
	}  // Anonymous namespace

	void Internal::Init()
	{
		s_initialized = true;
	}

	bool Internal::ProcessInputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// If input system has not been initialized, do nothing
		if (!s_initialized)
			return false;


		switch (uMsg)
		{

		// ----- Mouse move events -----

		case WM_MOUSEMOVE: [[fallthrough]];
		case WM_NCMOUSEMOVE:
		{
			POINT mousePos = { LOWORD(lParam), HIWORD(lParam) };
			if (uMsg == WM_MOUSEMOVE)
				::ClientToScreen(hWnd, &mousePos);
			if (uMsg == WM_NCMOUSEMOVE)
				::ScreenToClient(hWnd, &mousePos);

			Internal::InputEvent e;
			e.Type = Internal::InputEventType::MouseMove;
			e.MouseMove.PosX = static_cast<F32>(mousePos.x);
			e.MouseMove.PosY = static_cast<F32>(mousePos.y);

			s_inputEvents.push(e);
			NUI_LOG(Info, Input, "Mouse (x: ", mousePos.x, ", y: ", mousePos.y, ")");
			return true;
		}

		// ----- Mouse Button down events -----

		case WM_LBUTTONDOWN:   [[fallthrough]];
		case WM_LBUTTONDBLCLK: [[fallthrough]];
		case WM_RBUTTONDOWN:   [[fallthrough]];
		case WM_RBUTTONDBLCLK: [[fallthrough]];
		case WM_MBUTTONDOWN:   [[fallthrough]];
		case WM_MBUTTONDBLCLK: [[fallthrough]];
		case WM_XBUTTONDOWN:   [[fallthrough]];
		case WM_XBUTTONDBLCLK:
		{
			MouseButton button{};

			if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) 
				 button = MouseButton::Left;

			if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) 
				 button = MouseButton::Right;

			if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) 
				 button = MouseButton::Middle;

			if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK) 
				 button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? MouseButton::MouseX1 : MouseButton::MouseX2;
		
			Internal::InputEvent e;
			e.Type = Internal::InputEventType::MouseButton;
			e.MouseButton.Button = button;
			e.MouseButton.Pressed = true;

			s_inputEvents.push(e);
			return true;
		}

		// ----- Mouse Button up events -----

		case WM_LBUTTONUP: [[fallthrough]];
		case WM_RBUTTONUP: [[fallthrough]];
		case WM_MBUTTONUP: [[fallthrough]];
		case WM_XBUTTONUP:
		{
			MouseButton button{};

			if (uMsg == WM_LBUTTONUP)
				button = MouseButton::Left;

			if (uMsg == WM_RBUTTONUP)
				button = MouseButton::Right;

			if (uMsg == WM_MBUTTONUP)
				button = MouseButton::Middle;

			if (uMsg == WM_XBUTTONUP)
				button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? MouseButton::MouseX1 : MouseButton::MouseX2;

			Internal::InputEvent e;
			e.Type = Internal::InputEventType::MouseButton;
			e.MouseButton.Button = button;
			e.MouseButton.Pressed = false;
			return false;
		}

		// ----- Mouse Wheel events -----
		
		case WM_MOUSEWHEEL:
		{
			Internal::InputEvent e;
			e.Type = Internal::InputEventType::MouseWheel;
			e.MouseWheel.WheelX = 0.0f;
			e.MouseWheel.WheelY = GET_WHEEL_DELTA_WPARAM(wParam);
			return	true;
		}

		case WM_MOUSEHWHEEL:
		{
			Internal::InputEvent e;
			e.Type = Internal::InputEventType::MouseWheel;
			e.MouseWheel.WheelX = GET_WHEEL_DELTA_WPARAM(wParam);
			e.MouseWheel.WheelY = 0.0f;
			return true;
		}

		// ----- Keyboard events -----

		case WM_KEYDOWN:    [[fallthrough]];
		case WM_KEYUP:      [[fallthrough]];
		case WM_SYSKEYDOWN: [[fallthrough]];
		case WM_SYSKEYUP:
		{
			const bool isKeyDown = (uMsg== WM_KEYDOWN || uMsg== WM_SYSKEYDOWN);

		}
		}

		return false;
	}
}