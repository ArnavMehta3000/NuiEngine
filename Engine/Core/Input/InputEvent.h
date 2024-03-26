#pragma once
#include "Core/Input/KeyCode.h"

namespace Nui::Input
{
	namespace Internal
	{
		enum class InputEventType
		{
			None = 0,
			MouseMove,
			MouseMoveRaw,
			MouseButton,
			MouseWheel,
			Key,
		};

		struct MouseMoveInputEvent
		{
			F32 PosX = 0.0f;
			F32 PosY = 0.0f;
		};

		struct MouseButtonInputEvent
		{
			MouseButton Button = MouseButton::None;
			Modifier Modifier = Modifier::None;
			bool Pressed = false;
		};

		struct MouseWheelInputEvent
		{
			F32 WheelX = 0.0f;
			F32 WheelY = 0.0f;
		};

		struct KeyInputEvent
		{
			KeyCode Key = KeyCode::None;
			Modifier Modifier = Modifier::None;
			bool Pressed = false;
		};


		struct InputEvent
		{
			InputEvent() : Type(InputEventType::None) {};

			InputEventType Type = InputEventType::None;
			union
			{
				MouseMoveInputEvent MouseMove;
				MouseButtonInputEvent MouseButton;
				MouseWheelInputEvent MouseWheel;
				KeyInputEvent Key;
			};
		};
	}
}