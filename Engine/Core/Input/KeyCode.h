#pragma once
#include "Core/Common/NuiWin.h"
#include "Core/Common/Types.h"

namespace Nui::Input
{
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

		KEYCODE_COUNT
	};

	enum Modifier : U64
	{
		MOD_None = 0,

		MOD_LShift   = 1 << 0,
		MOD_RShift   = 1 << 1,
		MOD_LControl = 1 << 2,
		MOD_RControl = 1 << 3,
		MOD_LAlt     = 1 << 4,
		MOD_RAlt     = 1 << 5,
		MOD_LSuper   = 1 << 6,
		MOD_RSuper   = 1 << 7,
	};

	inline constexpr U64 ConvertKeyCodeToArrayIndex(KeyCode keyCode)
	{
		switch (keyCode)
		{
		case KeyCode::LeftArrow:      return 0;
		case KeyCode::RightArrow:     return 1;
		case KeyCode::UpArrow:        return 2;
		case KeyCode::DownArrow:      return 3;
		case KeyCode::PageUp:         return 4;
		case KeyCode::PageDown:       return 5;
		case KeyCode::Home:           return 6;
		case KeyCode::End:            return 7;
		case KeyCode::Insert:         return 8;
		case KeyCode::Delete:         return 9;
		case KeyCode::Backspace:      return 10;
		case KeyCode::Space:          return 11;
		case KeyCode::Enter:          return 12;
		case KeyCode::Escape:         return 13;
		case KeyCode::LeftCtrl:       return 14;
		case KeyCode::LeftShift:      return 15;
		case KeyCode::LeftAlt:        return 16;
		case KeyCode::LeftSuper:      return 17;
		case KeyCode::RightCtrl:      return 18;
		case KeyCode::RightShift:     return 19;
		case KeyCode::RightAlt:       return 20;
		case KeyCode::RightSuper:     return 21;
		case KeyCode::Menu:           return 22;
		case KeyCode::Num0:           return 23;
		case KeyCode::Num1:           return 24;
		case KeyCode::Num2:           return 25;
		case KeyCode::Num3:           return 26;
		case KeyCode::Num4:           return 27;
		case KeyCode::Num5:           return 28;
		case KeyCode::Num6:           return 29;
		case KeyCode::Num7:           return 30;
		case KeyCode::Num8:           return 31;
		case KeyCode::Num9:           return 32;
		case KeyCode::A:              return 33;
		case KeyCode::B:              return 34;
		case KeyCode::C:              return 35;
		case KeyCode::D:              return 36;
		case KeyCode::E:              return 37;
		case KeyCode::F:              return 38;
		case KeyCode::G:              return 39;
		case KeyCode::H:              return 40;
		case KeyCode::I:              return 41;
		case KeyCode::J:              return 42;
		case KeyCode::K:              return 43;
		case KeyCode::L:              return 44;
		case KeyCode::M:              return 45;
		case KeyCode::N:              return 46;
		case KeyCode::O:              return 47;
		case KeyCode::P:              return 48;
		case KeyCode::Q:              return 49;
		case KeyCode::R:              return 50;
		case KeyCode::S:              return 51;
		case KeyCode::T:              return 52;
		case KeyCode::U:              return 53;
		case KeyCode::V:              return 54;
		case KeyCode::W:              return 55;
		case KeyCode::X:              return 56;
		case KeyCode::Y:              return 57;
		case KeyCode::Z:              return 58;
		case KeyCode::F1:             return 59;
		case KeyCode::F2:             return 60;
		case KeyCode::F3:             return 61;
		case KeyCode::F4:             return 62;
		case KeyCode::F5:             return 63;
		case KeyCode::F6:             return 64;
		case KeyCode::F7:             return 65;
		case KeyCode::F8:             return 66;
		case KeyCode::F9:             return 67;
		case KeyCode::F10:            return 68;
		case KeyCode::F11:            return 69;
		case KeyCode::F12:            return 70;
		case KeyCode::F13:            return 71;
		case KeyCode::F14:            return 72;
		case KeyCode::F15:            return 73;
		case KeyCode::F16:            return 74;
		case KeyCode::F17:            return 75;
		case KeyCode::F18:            return 76;
		case KeyCode::F19:            return 77;
		case KeyCode::F20:            return 78;
		case KeyCode::F21:            return 79;
		case KeyCode::F22:            return 80;
		case KeyCode::F23:            return 81;
		case KeyCode::F24:            return 82;
		case KeyCode::Apostrophe:     return 83;
		case KeyCode::Comma:          return 84;
		case KeyCode::Minus:          return 85;
		case KeyCode::Period:         return 86;
		case KeyCode::Slash:          return 87;
		case KeyCode::Semicolon:      return 88;
		case KeyCode::Equal:          return 89;
		case KeyCode::LBracket:       return 90;
		case KeyCode::RBracket:       return 91;
		case KeyCode::Backslash:      return 92;
		case KeyCode::GraveAccent:    return 93;
		case KeyCode::CapsLock:       return 94;
		case KeyCode::ScrollLock:     return 95;
		case KeyCode::NumLock:        return 96;
		case KeyCode::PrintScreen:    return 97;
		case KeyCode::NumPad0:        return 98;
		case KeyCode::NumPad1:        return 99;
		case KeyCode::NumPad2:        return 100;
		case KeyCode::NumPad3:        return 101;
		case KeyCode::NumPad4:        return 102;
		case KeyCode::NumPad5:        return 103;
		case KeyCode::NumPad6:        return 104;
		case KeyCode::NumPad7:        return 105;
		case KeyCode::NumPad8:        return 106;
		case KeyCode::NumPad9:        return 107;
		case KeyCode::NumPadDecimal:  return 108;
		case KeyCode::NumPadDivide:   return 109;
		case KeyCode::NumPadMultiply: return 110;
		case KeyCode::NumPadSubtract: return 111;
		case KeyCode::NumPadAdd:      return 112;
		case KeyCode::KEYCODE_COUNT:  return 113;
		}

		// Return out of bounds index
		return ConvertKeyCodeToArrayIndex(KeyCode::KEYCODE_COUNT) + 1;
	}

}