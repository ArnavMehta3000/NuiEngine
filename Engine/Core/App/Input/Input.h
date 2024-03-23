#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/App/Input/Key.h"
#include <map>

namespace Nui::Input
{
	namespace Internal
	{
		/**
		 * @brief Initialize input subsystem
		 * @param hWnd 
		 */
		void Init(HWND hWnd);

		/**
		 * @brief Process all Win32 input messages
		 * @param hWnd Handle to the window
		 * @param uMsg WndProc uMsg
		 * @param wParam WndProc wParam
		 * @param lParam WndProc lParam
		 * @return 0 is message was processed
		 */
		LRESULT ProcessAll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		/**
		 * @brief Reset input state
		 * @param resetHeldKeys Reset held keys
		 */
		void Reset(bool resetHeldKeys = false);
		
		/**
		 * @brief Perform input update. Called before engine update
		 * @param windowIsFocused Is the window focused
		 */
		void PreUpdate(bool windowIsFocused);
		
		/**
		 * @brief Perform input update. Called after engine update
		 */
		void PostUpdate();
	}

	/**
	 * @brief Input modifier keys
	 */
	struct Modifier
	{
		/**
		 * @brief Is the alt key held
		 */
		bool HasAlt  { false };

		/**
		 * @brief Is the ctrl key held
		 */
		bool HasCtrl { false };
		
		/**
		 * @brief Is the shift key held
		 */
		bool HasShift{ false };
	};

	/**
	 * @brief Contains the state of a key
	 */
	struct KeyState
	{
		/**
		 * @brief Key code
		 */
		KeyCode  Code      { KeyCode::KEY_NULL };

		/**
		 * @brief Modifier keys
		 */
		Modifier Modifier  { };

		/**
		 * @brief Is the key held
		 */
		bool     IsHeld    { false };

		/**
		 * @brief Is the key pressed
		 */
		bool     IsPressed { false };
		
		/**
		 * @brief Is the key released
		 */
		bool     IsReleased{ false };
	};

	/**
	 * @brief Contains the state of the mouse
	 */
	struct Mouse
	{
		/**
		 * @brief Structure containing mouse position
		 */
		struct Point 
		{
			/**
			 * @brief X coordinate
			 */
			I32 X{ 0 };

			/**
			 * @brief Y coordinate
			 */
			I32 Y{ 0 }; 
		};

		/**
		 * @brief Structure containing mouse wheel state
		 */
		struct WheelData
		{
			/**
			 * @brief Mouse wheel delta (direction)
			 */
			I32      Delta   { 0 };

			/**
			 * @brief Modifier key states when the mouse wheel was triggered
			 */
			Modifier Modifier{ };

			/**
			 * @brief Mouse position when the mouse wheel was triggered
			 */
			Point    Position{ };
		};

		/**
		 * @brief Structure containing mouse button state
		 */
		struct ButtonState
		{
			/**
			 * @brief State of the mouse button
			 */
			KeyState State   { };

			/**
			 * @brief Mouse position when the mouse button was triggered
			 */
			Point    Position{ };
		};

		/**
		 * @brief Initialize mouse
		 */
		Mouse();

		/**
		 * @brief Mouse position
		 */
		Point     Position;
		
		/**
		 * @brief Raw mouse position
		 */
		Point     Raw;

		/**
		 * @brief Mouse vertical wheel
		 */
		WheelData Wheel;

		/**
		 * @brief Mouse horizontal wheel
		 */
		WheelData WheelH;

		/**
		 * @brief Mouse button to state map
		 */
		std::map<KeyCode, ButtonState> Buttons;
	};

	/**
	 * @brief Get the state of a keyboard key
	 * @param keyCode Key code to get
	 * @return State of the key
	 */
	KeyState GetKeyState(KeyCode keyCode);
	
	/**
	 * @brief Get the state of a mouse button
	 * @param keyCode Key code of the button to get
	 * @return State of the button
	 */
	Mouse::ButtonState GetMouseState(KeyCode keyCode);

	/**
	 * @brief Get the current mouse position
	 * @return Current mouse position
	 */
	Mouse::Point GetMousePosition();

	/**
	 * @brief Get the raw mouse delta
	 * @return Raw mouse delta
	 */
	Mouse::Point GetMouseRawDelta();

	/**
	 * @brief Get the state of the vertical mouse wheel
	 * @return State of the mouse wheel
	 */
	Mouse::WheelData GetMouseWheel();

	/**
	 * @brief Get the state of the horizontal mouse wheel
	 * @return State of the mouse wheel
	 */
	Mouse::WheelData GetMouseWheelH();
}