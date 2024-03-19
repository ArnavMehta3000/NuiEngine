#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Input/Key.h"
#include <map>

namespace Nui::Input
{
	namespace Internal
	{
		void Init(HWND hWnd);
		LRESULT ProcessAll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void Reset();
		void PreUpdate(bool windowIsFocused);
		void PostUpdate();
	}

	struct Modifier
	{
		bool HasAlt  { false };
		bool HasCtrl { false };
		bool HasShift{ false };
	};

	struct KeyState
	{
		KeyCode  Code      { KeyCode::KEY_NULL };
		Modifier Modifier  { };
		bool     IsHeld    { false };
		bool     IsPressed { false };
		bool     IsReleased{ false };
	};

	
	struct Mouse
	{
		struct Point { I32 X{ 0 }, Y{ 0 }; };
		;
		struct WheelData
		{
			I32      Delta   { 0 };
			Modifier Modifier{ };
			Point    Position{ };
		};

		struct ButtonState
		{
			KeyState State   { };
			Modifier Modifier{ };
			Point    Position{ };
		};

		Mouse();


		Point     Position;
		Point     Raw;
		WheelData Wheel;
		WheelData WheelH;
		std::map<KeyCode, ButtonState> Buttons;
	};

	KeyState GetKeyState(KeyCode keyCode);
	Mouse::ButtonState GetMouseState(KeyCode keyCode);

	Mouse::Point GetMousePosition();
	Mouse::Point GetMouseRawPosition();
	Mouse::WheelData GetMouseWheel();
	Mouse::WheelData GetMouseWheelH();
}