#pragma once
#include "Core/Common/NuiWin.h"
#include "Core/Common/Types.h"

namespace Nui::Input
{
// There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED, we assign it an arbitrary value to make code more readable (VK_ codes go up to 255)
// From ImGui
#define NUI_VK_NUMPAD_ENTER      (VK_RETURN + 256)

	enum class KeyCode : U64
	{
		None           = 0,

		LeftArrow      = VK_LEFT,
		RightArrow     = VK_RIGHT,
		UpArrow        = VK_UP,
		DownArrow      = VK_DOWN,
		PageUp         = VK_PRIOR,
		PageDown       = VK_NEXT,
		Home           = VK_HOME,
		End            = VK_END,
		Insert         = VK_INSERT,
		Delete         = VK_DELETE,
		Backspace      = VK_BACK,
		Space          = VK_SPACE,
		Enter          = VK_RETURN,
		Escape         = VK_ESCAPE,
		LeftCtrl       = VK_LCONTROL,
		LeftShift      = VK_LSHIFT,
		LeftAlt        = VK_LMENU,
		LeftSuper      = VK_LWIN,
		RightCtrl      = VK_RCONTROL,
		RightShift     = VK_RSHIFT,
		RightAlt       = VK_RMENU,
		RightSuper     = VK_RWIN,
		Menu           = VK_APPS,

		Num0           = '0',
		Num1           = '1',
		Num2           = '2',
		Num3           = '3',
		Num4           = '4',
		Num5           = '5',
		Num6           = '6',
		Num7           = '7',
		Num8           = '8',
		Num9           = '9',

		A              = 'A',
		B              = 'B',
		C              = 'C',
		D              = 'D',
		E              = 'E',
		F              = 'F',
		G              = 'G',
		H              = 'H',
		I              = 'I',
		J              = 'J',
		K              = 'K',
		L              = 'L',
		M              = 'M',
		N              = 'N',
		O              = 'O',
		P              = 'P',
		Q              = 'Q',
		R              = 'R',
		S              = 'S',
		T              = 'T',
		U              = 'U',
		V              = 'V',
		W              = 'W',
		X              = 'X',
		Y              = 'Y',
		Z              = 'Z',

		F1             = VK_F1,
		F2             = VK_F2,
		F3             = VK_F3,
		F4             = VK_F4,
		F5             = VK_F5,
		F6             = VK_F6,
		F7             = VK_F7,
		F8             = VK_F8,
		F9             = VK_F9,
		F10            = VK_F10,
		F11            = VK_F11,
		F12            = VK_F12,
		F13            = VK_F13,
		F14            = VK_F14,
		F15            = VK_F15,
		F16            = VK_F16,
		F17            = VK_F17,
		F18            = VK_F18,
		F19            = VK_F19,
		F20            = VK_F20,
		F21            = VK_F21,
		F22            = VK_F22,
		F23            = VK_F23,
		F24            = VK_F24,

		Apostrophe     = VK_OEM_7,
		Comma          = VK_OEM_COMMA,
		Minus          = VK_OEM_MINUS,
		Period         = VK_OEM_PERIOD,
		Slash          = VK_OEM_2,
		Semicolon      = VK_OEM_1,
		Equal          = VK_OEM_PLUS,
		LBracket       = VK_OEM_4,
		RBracket       = VK_OEM_6,
		Backslash      = VK_OEM_5,
		GraveAccent    = VK_OEM_3,
		CapsLock       = VK_CAPITAL,
		ScrollLock     = VK_SCROLL,
		NumLock        = VK_NUMLOCK,
		PrintScreen    = VK_SNAPSHOT,

		NumPad0        = VK_NUMPAD0,
		NumPad1        = VK_NUMPAD1,
		NumPad2        = VK_NUMPAD2,
		NumPad3        = VK_NUMPAD3,
		NumPad4        = VK_NUMPAD4,
		NumPad5        = VK_NUMPAD5,
		NumPad6        = VK_NUMPAD6,
		NumPad7        = VK_NUMPAD7,
		NumPad8        = VK_NUMPAD8,
		NumPad9        = VK_NUMPAD9,
		NumPadDecimal  = VK_DECIMAL,
		NumPadDivide   = VK_DIVIDE,
		NumPadMultiply = VK_MULTIPLY,
		NumPadSubtract = VK_SUBTRACT,
		NumPadAdd      = VK_ADD,
		NumPadEnter    = NUI_VK_NUMPAD_ENTER,

		KEYCODE_COUNT
	};

	enum class MouseButton
	{
		None = 0,

		Left,
		Right,
		Middle,
		MouseX1,
		MouseX2,
	};

	enum class Modifier : U64
	{
		None = 0,

		Shift   = 1 << 0,
		Control = 1 << 1,
		Alt     = 1 << 2,
		Super   = 1 << 3,
	};
}