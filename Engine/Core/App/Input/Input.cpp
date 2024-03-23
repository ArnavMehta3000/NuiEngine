#include "Input.h"
#include <bitset>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

namespace Nui::Input
{
	namespace Internal
	{
		// Global input state
		std::unique_ptr<Mouse>              g_mouse{ nullptr };
		std::bitset<KeyCode::KEY_MAX_COUNT> g_pressedKeys;
		std::bitset<KeyCode::KEY_MAX_COUNT> g_releasedKeys;
		std::bitset<KeyCode::KEY_MAX_COUNT> g_heldKeys;


		bool IsMouseKey(KeyCode keyCode)
		{
			return keyCode < KeyCode::KEY_XBUTTON2;
		}

		void Init(HWND hWnd)
		{
			NUI_LOG(Debug, Input, "Initializing input...");
			g_mouse = std::make_unique<Mouse>();

			// Register for raw input
			NUI_LOG(Debug, Input, "Registering raw input...");
			RAWINPUTDEVICE Rid[1];
			Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
			Rid[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
			Rid[0].dwFlags     = RIDEV_INPUTSINK;
			Rid[0].hwndTarget  = hWnd;

			if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])))
			{
				NUI_LOG(Error, Input, "Failed to register raw input devices");
				NUI_LOG(Debug, Input, "Input initialization completed with errors");
			}
			else
			{
				NUI_LOG(Debug, Input, "Input initialization completed successfully");
			}
		}

		LRESULT ProcessMouseButton(KeyCode keyCode, bool pressed, bool hasCtrl, bool hasShift, LPARAM lParam)
		{
			NUI_ASSERT(IsMouseKey(keyCode),
				"ProcessMouseButton should only be called with mouse keycodes");

			if (g_mouse)
			{
				auto& state = g_mouse->Buttons[keyCode];

				POINTS p                      = MAKEPOINTS(lParam);
				state.Position.X              = p.x;
				state.Position.Y              = p.y;
				state.State.Modifier.HasAlt   = (bool)(::GetKeyState(VK_MENU) & 0x8000);
				state.State.Modifier.HasCtrl  = hasCtrl;
				state.State.Modifier.HasShift = hasShift;

				if (pressed)
				{
					if (state.State.IsHeld)
					{
						state.State.IsPressed  = true;
						state.State.IsHeld     = true;
						state.State.IsReleased = false;
					}
				}
				else
				{
					state.State.IsPressed  = false;
					state.State.IsHeld     = false;
					state.State.IsReleased = false;
				}
			}

			return 0;
		}

		LRESULT ProcessAll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{

			switch (uMsg)
			{
			case WM_SYSKEYDOWN:
				[[fallthrough]];
			case WM_KEYDOWN:
			{
				std::size_t key{ wParam };

				if (!g_heldKeys.test(key))
				{
					g_pressedKeys.set(key, true);
					g_heldKeys.set(key, true);
					g_releasedKeys.set(key, false);
				}
				else
				{
					g_pressedKeys.set(key, false); // Set g_pressedKeys to false if the key is already held
				}
				return 0;
			}

			case WM_SYSKEYUP:
				[[fallthrough]];
			case WM_KEYUP:
			{
				std::size_t key{ wParam };

				g_pressedKeys.set(key, false);
				g_heldKeys.set(key, false);
				g_releasedKeys.set(key, true);
				return 0;
			}

			case WM_INPUT:
			{
				U32 dwSize = sizeof(RAWINPUT);
				static BYTE lpb[sizeof(RAWINPUT)];

				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw && raw->header.dwType == RIM_TYPEMOUSE)
				{
					if (g_mouse)
					{
						g_mouse->Raw.X = raw->data.mouse.lLastX;
						g_mouse->Raw.Y = raw->data.mouse.lLastY;
					}
				}
				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				return ProcessMouseButton(
					KEY_LBUTTON,
					true,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_MBUTTONDOWN:
			{
				return ProcessMouseButton(
					KEY_MBUTTON,
					true,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				return ProcessMouseButton(
					KEY_RBUTTON,
					true,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_LBUTTONUP:
			{
				return ProcessMouseButton(
					KEY_LBUTTON,
					false,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_MBUTTONUP:
			{
				return ProcessMouseButton(
					KEY_MBUTTON,
					false,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_RBUTTONUP:
			{
				return ProcessMouseButton(
					KEY_RBUTTON,
					false,
					wParam & MK_CONTROL,
					wParam & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_XBUTTONDOWN:
			{
				return ProcessMouseButton(
					KeyCode((HIWORD(wParam))),
					true,
					LOWORD(wParam) & MK_CONTROL,
					LOWORD(wParam) & MK_SHIFT,
					lParam);
				return 0;
			}

			case WM_XBUTTONUP:
			{
				return ProcessMouseButton(
					KeyCode((HIWORD(wParam))),
					false,
					LOWORD(wParam) & MK_CONTROL,
					LOWORD(wParam) & MK_SHIFT,
					lParam);
			}

			case WM_MOUSEMOVE:
			{
				if (g_mouse)
				{
					POINTS p = MAKEPOINTS(lParam);
					g_mouse->Position.X = p.x;
					g_mouse->Position.Y = p.y;
				}
				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				if (g_mouse)
				{
					g_mouse->Wheel.Delta    = GET_WHEEL_DELTA_WPARAM(wParam);
					g_mouse->Wheel.Modifier =
					{
						.HasAlt   = bool(::GetKeyState(VK_MENU) & 0x8000),
						.HasCtrl  = bool(LOWORD(wParam) & MK_CONTROL),
						.HasShift = bool(LOWORD(wParam) & MK_SHIFT)
					};

					POINTS p = MAKEPOINTS(lParam);
					g_mouse->Wheel.Position.X = p.x;
					g_mouse->Wheel.Position.Y = p.y;
				}
				return 0;
			}

			case WM_MOUSEHWHEEL:
			{
				if (g_mouse)
				{
					g_mouse->WheelH.Delta    = GET_WHEEL_DELTA_WPARAM(wParam);
					g_mouse->WheelH.Modifier =
					{
						.HasAlt   = bool(::GetKeyState(VK_MENU) & 0x8000),
						.HasCtrl  = bool(LOWORD(wParam) & MK_CONTROL),
						.HasShift = bool(LOWORD(wParam) & MK_SHIFT)
					};

					POINTS p = MAKEPOINTS(lParam);
					g_mouse->WheelH.Position.X = p.x;
					g_mouse->WheelH.Position.Y = p.y;
				}
				return 0;
			}
			}


			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		void Reset(bool resetHeldKeys)
		{
			// Reset keyboard
			g_pressedKeys.reset();
			g_releasedKeys.reset();
			if (resetHeldKeys)
			{
				g_heldKeys.reset();
			}

			// Reset mouse
			if (g_mouse)
			{
				// Reset mouse buttons
				for (auto& [button, state] : g_mouse->Buttons)
				{
					state.State.IsHeld     = false;
					state.State.IsPressed  = false;
					state.State.IsReleased = false;
					state.State.Modifier   = { false, false, false };
					state.Position         = { 0, 0 };
				}

				// Reset mouse wheels
				g_mouse->Wheel = {};
				g_mouse->WheelH = {};
			}
		}

		void PreUpdate(bool windowIsFocused)
		{
			if (!windowIsFocused)
			{
				Reset(true);
			}
		}

		void PostUpdate()
		{
			Reset();
		}
	}


	KeyState GetKeyState(KeyCode keyCode)
	{
		NUI_ASSERT(!Internal::IsMouseKey(keyCode),
			"GetKeyState should not be called with mouse keycodes. Use GetMouseState instead");

		std::size_t key{ keyCode };
		return KeyState
		{
			.Code         = keyCode,
			.Modifier     = 
			{
				.HasAlt   = (bool)(::GetKeyState(VK_MENU) & 0x8000),
				.HasCtrl  = (bool)(::GetKeyState(VK_CONTROL) & 0x8000),
				.HasShift = (bool)(::GetKeyState(VK_SHIFT) & 0x8000)
			},
			.IsHeld       = Internal::g_heldKeys.test(key),
			.IsPressed    = Internal::g_pressedKeys.test(key),
			.IsReleased   = Internal::g_releasedKeys.test(key)
		};
	}

	Mouse::Mouse()
	{
		Buttons[KeyCode::KEY_LBUTTON]  = { ButtonState{KeyState{ KeyCode::KEY_LBUTTON  } } };
		Buttons[KeyCode::KEY_RBUTTON]  = { ButtonState{KeyState{ KeyCode::KEY_RBUTTON  } } };
		Buttons[KeyCode::KEY_MBUTTON]  = { ButtonState{KeyState{ KeyCode::KEY_MBUTTON  } } };
		Buttons[KeyCode::KEY_XBUTTON1] = { ButtonState{KeyState{ KeyCode::KEY_XBUTTON1 } } };
		Buttons[KeyCode::KEY_XBUTTON2] = { ButtonState{KeyState{ KeyCode::KEY_XBUTTON2 } } };
	}

	Mouse::ButtonState GetMouseState(KeyCode keyCode)
	{
		NUI_ASSERT(Internal::IsMouseKey(keyCode),
			"GetMouseState should only be called with mouse keycodes");

		NUI_ASSERT(Internal::g_mouse.get(), "GetMouseState should only be called after mouse is initialized");
		
		return Internal::g_mouse->Buttons[keyCode];
	}

	Mouse::Point GetMousePosition()
	{
		NUI_ASSERT(Internal::g_mouse.get(), "GetMousePosition should only be called after mouse is initialized");

		return Internal::g_mouse->Position;
	}

	Mouse::Point GetMouseRawDelta()
	{
		NUI_ASSERT(Internal::g_mouse.get(), "GetMousePosition should only be called after mouse is initialized");
		
		return Internal::g_mouse->Raw;
	}

	Mouse::WheelData GetMouseWheel()
	{
		NUI_ASSERT(Internal::g_mouse.get(), "GetMousePosition should only be called after mouse is initialized");

		return Internal::g_mouse->Wheel;
	}

	Mouse::WheelData GetMouseWheelH()
	{
		NUI_ASSERT(Internal::g_mouse.get(), "GetMousePosition should only be called after mouse is initialized");

		return Internal::g_mouse->WheelH;
	}
}