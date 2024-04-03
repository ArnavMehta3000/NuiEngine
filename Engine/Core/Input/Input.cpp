#include "Input.h"
#include "Core/App/Window.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

namespace Nui::Input
{
	namespace  // Anonymous namespace
	{
		static bool                      s_initialized{ false };
		static std::unique_ptr<Mouse>    s_mouse{ nullptr };
		static std::unique_ptr<Keyboard> s_keyboard{ nullptr };



		static bool IsVKPressed(I32 vk)
		{
			return (::GetKeyState(vk) & 0x8000) != 0;
		}

		static Modifier GetModifiers()
		{
			U64 m = 0;  // Modifier::None
			if (IsVKPressed(VK_LSHIFT))   
				m |= Modifier::MOD_LShift;
			if (IsVKPressed(VK_RSHIFT))   
				m |= Modifier::MOD_RShift;
			if (IsVKPressed(VK_LCONTROL)) 
				m |= Modifier::MOD_LControl;
			if (IsVKPressed(VK_RCONTROL)) 
				m |= Modifier::MOD_RControl;
			if (IsVKPressed(VK_LMENU))    
				m |= Modifier::MOD_LAlt;
			if (IsVKPressed(VK_RMENU))    
				m |= Modifier::MOD_RAlt;
			if (IsVKPressed(VK_LWIN))     
				m |= Modifier::MOD_LSuper;
			if (IsVKPressed(VK_RWIN))     
				m |= Modifier::MOD_RSuper;

			return static_cast<Modifier>(m);
		}

		bool ProcessMouseButton(Mouse::Button btn, bool pressed, LPARAM lParam)
		{
			NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");

			if (s_mouse)
			{
				POINTS p = MAKEPOINTS(lParam);
				s_mouse->Position.X = p.x;
				s_mouse->Position.Y = p.y;

				Mouse::ButtonState& state = s_mouse->ButtonStates[Mouse::ConvertMouseButtonToArrayIndex(btn)];
				NUI_ASSERT(state.Btn == btn, "Input button and mouse state button mismatch!");

				state.Modifier = GetModifiers();

				if (pressed)
				{
					if (!state.IsHeld)
					{
						state.IsPressed  = true;
						state.IsHeld     = true;
						state.IsReleased = false;
					}
					else
					{
						state.IsPressed = false;  // Set pressed to false if the button is already held
					}
				}
				else
				{
					state.IsPressed  = false;
					state.IsHeld     = false;
					state.IsReleased = true;
				}

				return true;
			}

			return false;
		}

		bool ProcessKeyboard(U64 vk, bool pressed)
		{
			bool handled = false;
			auto doVKDown = [&](U64 vk)
			{
				Keyboard::KeyState& state = s_keyboard->KeyStates[ConvertKeyCodeToArrayIndex((KeyCode)VK_LSHIFT)];

				if (!state.IsHeld)
				{
					state.IsPressed  = true;
					state.IsHeld     = true;
					state.IsReleased = false;
				}
				else
				{
					state.IsPressed = false;  // Set pressed to false if the key is already held
				}
				state.Modifier = GetModifiers();
			};

			auto doVKUp = [&](U64 vk)
			{
				Keyboard::KeyState& state = s_keyboard->KeyStates[ConvertKeyCodeToArrayIndex((KeyCode)VK_LSHIFT)];

				state.IsPressed  = false;
				state.IsHeld     = false;
				state.IsReleased = true;
				state.Modifier   = GetModifiers();
			};

			if (vk == VK_SHIFT)
			{
				if (IsVKPressed(VK_LSHIFT) == pressed) doVKDown(VK_LSHIFT);
				else doVKUp(VK_LSHIFT);
				if (IsVKPressed(VK_RSHIFT) == pressed) doVKDown(VK_RSHIFT);
				else doVKUp(VK_RSHIFT);
				handled = true;
			}
			if (vk == VK_CONTROL)
			{
				if (IsVKPressed(VK_LCONTROL) == pressed) doVKDown(VK_LCONTROL);
				else doVKUp(VK_LCONTROL);
				if (IsVKPressed(VK_RCONTROL) == pressed) doVKDown(VK_RCONTROL);
				else doVKUp(VK_RCONTROL);
				handled = true;
			}
			if (vk == VK_MENU)
			{
				if (IsVKPressed(VK_LMENU) == pressed) doVKDown(VK_LMENU);
				else doVKUp(VK_LMENU);
				if (IsVKPressed(VK_RMENU) == pressed) doVKDown(VK_RMENU);
				else doVKUp(VK_RMENU);
				handled = true;
			}
			return handled;
		}

		void Reset(bool resetHeld = false)
		{
			[[likely]]
			if (s_keyboard)  // Reset keyboard
			{
				for (auto& state: s_keyboard->KeyStates)
				{
					state.IsPressed  = false;
					state.IsReleased = false;
					if (resetHeld)
					{
						state.IsHeld = false;
					}
				}
			}

			[[likely]]
			if (s_mouse) // Reset mouse
			{
				for (auto& state: s_mouse->ButtonStates)
				{
					state.IsPressed  = false;
					state.IsReleased = false;
					if (resetHeld)
					{
						state.IsHeld = false;
					}
				}
				s_mouse->WheelH = Mouse::WheelInfo();
				s_mouse->WheelV = Mouse::WheelInfo();
			}
		}
	}  // Anonymous namespace

	constexpr Mouse::Mouse()
		: Position()
		, RawDelta()
		, WheelH()
		, WheelV()
	{
		ButtonStates[0] = ButtonState(Mouse::Button::Left);
		ButtonStates[1] = ButtonState(Mouse::Button::Right);
		ButtonStates[2] = ButtonState(Mouse::Button::Middle);
		ButtonStates[3] = ButtonState(Mouse::Button::MouseX1);
		ButtonStates[4] = ButtonState(Mouse::Button::MouseX2);
	}

	constexpr Keyboard::Keyboard()
	{
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LeftArrow)]      = KeyState(KeyCode::LeftArrow);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RightArrow)]     = KeyState(KeyCode::RightArrow);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::UpArrow)]        = KeyState(KeyCode::UpArrow);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::DownArrow)]      = KeyState(KeyCode::DownArrow);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::PageUp)]         = KeyState(KeyCode::PageUp);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::PageDown)]       = KeyState(KeyCode::PageDown);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Home)]           = KeyState(KeyCode::Home);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::End)]            = KeyState(KeyCode::End);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Insert)]         = KeyState(KeyCode::Insert);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Delete)]         = KeyState(KeyCode::Delete);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Backspace)]      = KeyState(KeyCode::Backspace);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Space)]          = KeyState(KeyCode::Space);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Enter)]          = KeyState(KeyCode::Enter);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Escape)]         = KeyState(KeyCode::Escape);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LeftCtrl)]       = KeyState(KeyCode::LeftCtrl);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LeftShift)]      = KeyState(KeyCode::LeftShift);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LeftAlt)]        = KeyState(KeyCode::LeftAlt);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LeftSuper)]      = KeyState(KeyCode::LeftSuper);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RightCtrl)]      = KeyState(KeyCode::RightCtrl);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RightShift)]     = KeyState(KeyCode::RightShift);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RightAlt)]       = KeyState(KeyCode::RightAlt);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RightSuper)]     = KeyState(KeyCode::RightSuper);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Menu)]           = KeyState(KeyCode::Menu);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num0)]           = KeyState(KeyCode::Num0);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num1)]           = KeyState(KeyCode::Num1);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num2)]           = KeyState(KeyCode::Num2);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num3)]           = KeyState(KeyCode::Num3);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num4)]           = KeyState(KeyCode::Num4);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num5)]           = KeyState(KeyCode::Num5);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num6)]           = KeyState(KeyCode::Num6);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num7)]           = KeyState(KeyCode::Num7);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num8)]           = KeyState(KeyCode::Num8);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Num9)]           = KeyState(KeyCode::Num9);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::A)]              = KeyState(KeyCode::A);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::B)]              = KeyState(KeyCode::B);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::C)]              = KeyState(KeyCode::C);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::D)]              = KeyState(KeyCode::D);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::E)]              = KeyState(KeyCode::E);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F)]              = KeyState(KeyCode::F);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::G)]              = KeyState(KeyCode::G);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::H)]              = KeyState(KeyCode::H);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::I)]              = KeyState(KeyCode::I);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::J)]              = KeyState(KeyCode::J);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::K)]              = KeyState(KeyCode::K);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::L)]              = KeyState(KeyCode::L);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::M)]              = KeyState(KeyCode::M);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::N)]              = KeyState(KeyCode::N);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::O)]              = KeyState(KeyCode::O);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::P)]              = KeyState(KeyCode::P);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Q)]              = KeyState(KeyCode::Q);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::R)]              = KeyState(KeyCode::R);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::S)]              = KeyState(KeyCode::S);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::T)]              = KeyState(KeyCode::T);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::U)]              = KeyState(KeyCode::U);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::V)]              = KeyState(KeyCode::V);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::W)]              = KeyState(KeyCode::W);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::X)]              = KeyState(KeyCode::X);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Y)]              = KeyState(KeyCode::Y);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Z)]              = KeyState(KeyCode::Z);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F1)]             = KeyState(KeyCode::F1);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F2)]             = KeyState(KeyCode::F2);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F3)]             = KeyState(KeyCode::F3);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F4)]             = KeyState(KeyCode::F4);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F5)]             = KeyState(KeyCode::F5);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F6)]             = KeyState(KeyCode::F6);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F7)]             = KeyState(KeyCode::F7);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F8)]             = KeyState(KeyCode::F8);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F9)]             = KeyState(KeyCode::F9);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F10)]            = KeyState(KeyCode::F10);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F11)]            = KeyState(KeyCode::F11);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F12)]            = KeyState(KeyCode::F12);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F13)]            = KeyState(KeyCode::F13);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F14)]            = KeyState(KeyCode::F14);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F15)]            = KeyState(KeyCode::F15);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F16)]            = KeyState(KeyCode::F16);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F17)]            = KeyState(KeyCode::F17);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F18)]            = KeyState(KeyCode::F18);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F19)]            = KeyState(KeyCode::F19);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F20)]            = KeyState(KeyCode::F20);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F21)]            = KeyState(KeyCode::F21);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F22)]            = KeyState(KeyCode::F22);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F23)]            = KeyState(KeyCode::F23);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::F24)]            = KeyState(KeyCode::F24);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Apostrophe)]     = KeyState(KeyCode::Apostrophe);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Comma)]          = KeyState(KeyCode::Comma);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Minus)]          = KeyState(KeyCode::Minus);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Period)]         = KeyState(KeyCode::Period);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Slash)]          = KeyState(KeyCode::Slash);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Semicolon)]      = KeyState(KeyCode::Semicolon);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Equal)]          = KeyState(KeyCode::Equal);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::LBracket)]       = KeyState(KeyCode::LBracket);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::RBracket)]       = KeyState(KeyCode::RBracket);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::Backslash)]      = KeyState(KeyCode::Backslash);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::GraveAccent)]    = KeyState(KeyCode::GraveAccent);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::CapsLock)]       = KeyState(KeyCode::CapsLock);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::ScrollLock)]     = KeyState(KeyCode::ScrollLock);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumLock)]        = KeyState(KeyCode::NumLock);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::PrintScreen)]    = KeyState(KeyCode::PrintScreen);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad0)]        = KeyState(KeyCode::NumPad0);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad1)]        = KeyState(KeyCode::NumPad1);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad2)]        = KeyState(KeyCode::NumPad2);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad3)]        = KeyState(KeyCode::NumPad3);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad4)]        = KeyState(KeyCode::NumPad4);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad5)]        = KeyState(KeyCode::NumPad5);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad6)]        = KeyState(KeyCode::NumPad6);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad7)]        = KeyState(KeyCode::NumPad7);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad8)]        = KeyState(KeyCode::NumPad8);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPad9)]        = KeyState(KeyCode::NumPad9);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPadDecimal)]  = KeyState(KeyCode::NumPadDecimal);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPadDivide)]   = KeyState(KeyCode::NumPadDivide);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPadMultiply)] = KeyState(KeyCode::NumPadMultiply);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPadSubtract)] = KeyState(KeyCode::NumPadSubtract);
	   KeyStates[ConvertKeyCodeToArrayIndex(KeyCode::NumPadAdd)]      = KeyState(KeyCode::NumPadAdd);
	}

	bool Internal::ProcessInputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// If input system has not been initialized, register raw input devices but don't process input
		[[unlikely]]
		if (!s_initialized)
		{
			NUI_LOG(Debug, Input, "Initializing input system");
			s_mouse    = std::make_unique<Mouse>();
			s_keyboard = std::make_unique<Keyboard>();

			// Register for raw input
			NUI_LOG(Debug, Input, "Registering raw input...");
			RAWINPUTDEVICE Rid[1];
			Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
			Rid[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
			Rid[0].dwFlags     = RIDEV_INPUTSINK;
			Rid[0].hwndTarget  = hWnd;

			if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])))
			{
				NUI_LOG(Error, Input, "Failed to register raw input devices. ", GetWin32ErrorString(GetLastError()));
				NUI_LOG(Debug, Input, "Input initialization completed with errors");
			}
			else
			{
				NUI_LOG(Debug, Input, "Input initialization completed successfully");
			}
			
			// Consider input initialized even if registration failed (it's not fatal)
			s_initialized = true;
			return false;
		}

		// Check for window focus
		[[unlikely]]
		if (::GetFocus() != hWnd)
		{
			Reset(true);
			return false;
		}

		switch (uMsg)
		{
		case WM_SYSKEYDOWN: [[fallthrough]];
		case WM_KEYDOWN:
		{
			NUI_ASSERT((bool)s_keyboard, "Keyboard is not initialized");
			
			[[likely]]
			if ((U64)wParam < 256 && s_keyboard)
			{
				// Process some modifiers early
				if (ProcessKeyboard(wParam, true))
				{
					return true;
				}

				Keyboard::KeyState& state = s_keyboard->KeyStates[ConvertKeyCodeToArrayIndex((KeyCode)wParam)];
				NUI_ASSERT(state.Key == KeyCode(wParam), "WPARAM is not a KeyCode");

				if (!state.IsHeld)
				{
					state.IsPressed  = true;
					state.IsHeld     = true;
					state.IsReleased = false;
				}
				else
				{
					state.IsPressed = false;  // Set pressed to false if the key is already held
				}
				state.Modifier = GetModifiers();
				return true;
			}
			return false;
		}

		case WM_SYSKEYUP: [[fallthrough]];
		case WM_KEYUP:
		{
			NUI_ASSERT((bool)s_keyboard, "Keyboard is not initialized");

			[[likely]]
			if ((U64)wParam < 256 && s_keyboard)
			{
				// Process some modifiers early
				if (ProcessKeyboard(wParam, true))
				{
					return true;
				}

				Keyboard::KeyState& state = s_keyboard->KeyStates[ConvertKeyCodeToArrayIndex(KeyCode(wParam))];
				NUI_ASSERT(state.Key == KeyCode(wParam), "WPARAM is not a KeyCode");

				state.IsPressed  = false;
				state.IsHeld     = false;
				state.IsReleased = true;
				return true;
			}
			return false;
		}

		case WM_INPUT:
		{
			U32 dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			[[likely]]
			if (raw && raw->header.dwType == RIM_TYPEMOUSE)
			{
				NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
				[[likely]]
				if (s_mouse)
				{
					s_mouse->RawDelta.X = raw->data.mouse.lLastX;
					s_mouse->RawDelta.Y = raw->data.mouse.lLastY;
				}
			}
			return true;
		}

		case WM_LBUTTONDOWN:
		{
			return ProcessMouseButton(Mouse::Button::Left, true, lParam);
		}

		case WM_MBUTTONDOWN:
		{
			return ProcessMouseButton(Mouse::Button::Middle, true, lParam);
		}

		case WM_RBUTTONDOWN:
		{
			return ProcessMouseButton(Mouse::Button::Right, true, lParam);
		}

		case WM_LBUTTONUP:
		{
			return ProcessMouseButton(Mouse::Button::Left, false, lParam);
		}

		case WM_MBUTTONUP:
		{
			return ProcessMouseButton(Mouse::Button::Middle, false, lParam);
		}

		case WM_RBUTTONUP:
		{
			return ProcessMouseButton(Mouse::Button::Right, false, lParam);
		}

		case WM_XBUTTONDOWN:
		{
			Mouse::Button btn = (HIWORD(wParam) == XBUTTON1) ? Mouse::Button::MouseX1 : Mouse::Button::MouseX2;
			return ProcessMouseButton(btn, true, lParam);
		}

		case WM_XBUTTONUP:
		{
			Mouse::Button btn = (HIWORD(wParam) == XBUTTON1) ? Mouse::Button::MouseX1 : Mouse::Button::MouseX2;
			return ProcessMouseButton(btn, false, lParam);
		}

		case WM_MOUSEMOVE:
		{
			NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");

			if (s_mouse)
			{
				POINTS p = MAKEPOINTS(lParam);
				s_mouse->Position.X = p.x;
				s_mouse->Position.Y = p.y;
			}
			return true;
		}

		case WM_MOUSEWHEEL:
		{
			NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");

			if (s_mouse)
			{
				s_mouse->WheelV.Delta = GET_WHEEL_DELTA_WPARAM(wParam);
				s_mouse->WheelV.Modifier = GetModifiers();

				POINTS p = MAKEPOINTS(lParam);
				s_mouse->WheelV.Position.X = p.x;
				s_mouse->WheelV.Position.Y = p.y;
			}
			return true;
		}

		case WM_MOUSEHWHEEL:
		{
			NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
			if (s_mouse)
			{
				s_mouse->WheelH.Delta = GET_WHEEL_DELTA_WPARAM(wParam);
				s_mouse->WheelH.Modifier = GetModifiers();

				POINTS p = MAKEPOINTS(lParam);
				s_mouse->WheelH.Position.X = p.x;
				s_mouse->WheelH.Position.Y = p.y;
			}
			return true;
		}
		}

		return false;
	}

	void Internal::Update()
	{
		Reset(false);
	}




	const Mouse::Point& GetMousePosition()
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		return s_mouse->Position;
	}

	const Mouse::Point& GetMouseRawDelta()
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		return s_mouse->RawDelta;
	}

	const Mouse::WheelInfo& GetMouseWheelH()
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		return s_mouse->WheelH;
	}

	const Mouse::WheelInfo& GetMouseWheelV()
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		return s_mouse->WheelV;
	}

	const Mouse::ButtonState& GetMouseButton(Mouse::Button btn)
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		return s_mouse->ButtonStates[Mouse::ConvertMouseButtonToArrayIndex(btn)];
	}

	const Mouse::ButtonState& GetMouseButton(U32 btn)
	{
		NUI_ASSERT((bool)s_mouse, "Mouse is not initialized");
		NUI_ASSERT(btn < 5, "Mouse Button index out of range!");
		return s_mouse->ButtonStates[btn];
	}

	const Keyboard::KeyState& GetKeyState(KeyCode key)
	{
		NUI_ASSERT((bool)s_keyboard, "Keyboard is not initialized");
		return s_keyboard->KeyStates[ConvertKeyCodeToArrayIndex(key)];
	}

}