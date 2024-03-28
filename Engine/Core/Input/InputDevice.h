#pragma once
#include "Core/Input/KeyCode.h"
#include <array>

namespace Nui::Input
{
	struct Mouse
	{
		constexpr Mouse();

		enum class Button : U32
		{
			None = 0,

			Left    = MK_LBUTTON,   // Can be referenced using 0
			Right   = MK_RBUTTON,   // Can be referenced using 1
			Middle  = MK_MBUTTON,   // Can be referenced using 2
			MouseX1 = MK_XBUTTON1,  // Can be referenced using 3
			MouseX2 = MK_XBUTTON2,  // Can be referenced using 4
		};

		static constexpr U32 ConvertMouseButtonToArrayIndex(Mouse::Button btn)
		{
			switch (btn)
			{
			case Mouse::Button::Left:    return 0;
			case Mouse::Button::Right:   return 1;
			case Mouse::Button::Middle:  return 2;
			case Mouse::Button::MouseX1: return 3;
			case Mouse::Button::MouseX2: return 4;
			}

			// Return out of array bounds index
			return 5;
		}

		struct Point
		{
			constexpr Point(I32 x = 0, I32 y = 0) : X(x), Y(y) {}

			I32 X{ 0 };
			I32 Y{ 0 };
		};

		struct WheelInfo
		{
			constexpr WheelInfo()
				: Delta(0)
				, Modifier(Modifier::MOD_None)
				, Position()
			{}

			I32 Delta;
			Modifier Modifier;
			Point Position;
		};

		struct ButtonState
		{
			constexpr ButtonState(Mouse::Button btn = Mouse::Button::None)
				: Btn(btn)
				, Modifier(Modifier::MOD_None)
				, IsHeld(false)
				, IsPressed(false)
				, IsReleased(false)
			{}

			Button Btn;
			Modifier Modifier;
			bool IsHeld;
			bool IsPressed;
			bool IsReleased;
		};

		Point     Position;
		Point     RawDelta;
		WheelInfo WheelH;
		WheelInfo WheelV;
		std::array<ButtonState, 5> ButtonStates;
	};

	struct Keyboard
	{
		constexpr Keyboard();

		struct KeyState
		{
			constexpr KeyState(KeyCode key = KeyCode::None)
				: Key(key)
				, Modifier(Modifier::MOD_None)
				, IsPressed(false)
				, IsReleased(false)
				, IsHeld(false)
			{}

			KeyCode Key;
			Modifier Modifier;
			bool IsPressed;
			bool IsReleased;
			bool IsHeld;
		};

		std::array<KeyState, ConvertKeyCodeToArrayIndex(KeyCode::KEYCODE_COUNT)> KeyStates;
	};

	// Alias for Mouse::Button
	using MouseButton = Mouse::Button;
}