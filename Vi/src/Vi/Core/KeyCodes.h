#pragma once

namespace Vi
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define VI_KEY_SPACE           ::Vi::Key::Space
#define VI_KEY_APOSTROPHE      ::Vi::Key::Apostrophe    /* ' */
#define VI_KEY_COMMA           ::Vi::Key::Comma         /* , */
#define VI_KEY_MINUS           ::Vi::Key::Minus         /* - */
#define VI_KEY_PERIOD          ::Vi::Key::Period        /* . */
#define VI_KEY_SLASH           ::Vi::Key::Slash         /* / */
#define VI_KEY_0               ::Vi::Key::D0
#define VI_KEY_1               ::Vi::Key::D1
#define VI_KEY_2               ::Vi::Key::D2
#define VI_KEY_3               ::Vi::Key::D3
#define VI_KEY_4               ::Vi::Key::D4
#define VI_KEY_5               ::Vi::Key::D5
#define VI_KEY_6               ::Vi::Key::D6
#define VI_KEY_7               ::Vi::Key::D7
#define VI_KEY_8               ::Vi::Key::D8
#define VI_KEY_9               ::Vi::Key::D9
#define VI_KEY_SEMICOLON       ::Vi::Key::Semicolon     /* ; */
#define VI_KEY_EQUAL           ::Vi::Key::Equal         /* = */
#define VI_KEY_A               ::Vi::Key::A
#define VI_KEY_B               ::Vi::Key::B
#define VI_KEY_C               ::Vi::Key::C
#define VI_KEY_D               ::Vi::Key::D
#define VI_KEY_E               ::Vi::Key::E
#define VI_KEY_F               ::Vi::Key::F
#define VI_KEY_G               ::Vi::Key::G
#define VI_KEY_H               ::Vi::Key::H
#define VI_KEY_I               ::Vi::Key::I
#define VI_KEY_J               ::Vi::Key::J
#define VI_KEY_K               ::Vi::Key::K
#define VI_KEY_L               ::Vi::Key::L
#define VI_KEY_M               ::Vi::Key::M
#define VI_KEY_N               ::Vi::Key::N
#define VI_KEY_O               ::Vi::Key::O
#define VI_KEY_P               ::Vi::Key::P
#define VI_KEY_Q               ::Vi::Key::Q
#define VI_KEY_R               ::Vi::Key::R
#define VI_KEY_S               ::Vi::Key::S
#define VI_KEY_T               ::Vi::Key::T
#define VI_KEY_U               ::Vi::Key::U
#define VI_KEY_V               ::Vi::Key::V
#define VI_KEY_W               ::Vi::Key::W
#define VI_KEY_X               ::Vi::Key::X
#define VI_KEY_Y               ::Vi::Key::Y
#define VI_KEY_Z               ::Vi::Key::Z
#define VI_KEY_LEFT_BRACKET    ::Vi::Key::LeftBracket   /* [ */
#define VI_KEY_BACKSLASH       ::Vi::Key::Backslash     /* \ */
#define VI_KEY_RIGHT_BRACKET   ::Vi::Key::RightBracket  /* ] */
#define VI_KEY_GRAVE_ACCENT    ::Vi::Key::GraveAccent   /* ` */
#define VI_KEY_WORLD_1         ::Vi::Key::World1        /* non-US #1 */
#define VI_KEY_WORLD_2         ::Vi::Key::World2        /* non-US #2 */

/* Function keys */
#define VI_KEY_ESCAPE          ::Vi::Key::Escape
#define VI_KEY_ENTER           ::Vi::Key::Enter
#define VI_KEY_TAB             ::Vi::Key::Tab
#define VI_KEY_BACKSPACE       ::Vi::Key::Backspace
#define VI_KEY_INSERT          ::Vi::Key::Insert
#define VI_KEY_DELETE          ::Vi::Key::Delete
#define VI_KEY_RIGHT           ::Vi::Key::Right
#define VI_KEY_LEFT            ::Vi::Key::Left
#define VI_KEY_DOWN            ::Vi::Key::Down
#define VI_KEY_UP              ::Vi::Key::Up
#define VI_KEY_PAGE_UP         ::Vi::Key::PageUp
#define VI_KEY_PAGE_DOWN       ::Vi::Key::PageDown
#define VI_KEY_HOME            ::Vi::Key::Home
#define VI_KEY_END             ::Vi::Key::End
#define VI_KEY_CAPS_LOCK       ::Vi::Key::CapsLock
#define VI_KEY_SCROLL_LOCK     ::Vi::Key::ScrollLock
#define VI_KEY_NUM_LOCK        ::Vi::Key::NumLock
#define VI_KEY_PRINT_SCREEN    ::Vi::Key::PrintScreen
#define VI_KEY_PAUSE           ::Vi::Key::Pause
#define VI_KEY_F1              ::Vi::Key::F1
#define VI_KEY_F2              ::Vi::Key::F2
#define VI_KEY_F3              ::Vi::Key::F3
#define VI_KEY_F4              ::Vi::Key::F4
#define VI_KEY_F5              ::Vi::Key::F5
#define VI_KEY_F6              ::Vi::Key::F6
#define VI_KEY_F7              ::Vi::Key::F7
#define VI_KEY_F8              ::Vi::Key::F8
#define VI_KEY_F9              ::Vi::Key::F9
#define VI_KEY_F10             ::Vi::Key::F10
#define VI_KEY_F11             ::Vi::Key::F11
#define VI_KEY_F12             ::Vi::Key::F12
#define VI_KEY_F13             ::Vi::Key::F13
#define VI_KEY_F14             ::Vi::Key::F14
#define VI_KEY_F15             ::Vi::Key::F15
#define VI_KEY_F16             ::Vi::Key::F16
#define VI_KEY_F17             ::Vi::Key::F17
#define VI_KEY_F18             ::Vi::Key::F18
#define VI_KEY_F19             ::Vi::Key::F19
#define VI_KEY_F20             ::Vi::Key::F20
#define VI_KEY_F21             ::Vi::Key::F21
#define VI_KEY_F22             ::Vi::Key::F22
#define VI_KEY_F23             ::Vi::Key::F23
#define VI_KEY_F24             ::Vi::Key::F24
#define VI_KEY_F25             ::Vi::Key::F25

/* Keypad */
#define VI_KEY_KP_0            ::Vi::Key::KP0
#define VI_KEY_KP_1            ::Vi::Key::KP1
#define VI_KEY_KP_2            ::Vi::Key::KP2
#define VI_KEY_KP_3            ::Vi::Key::KP3
#define VI_KEY_KP_4            ::Vi::Key::KP4
#define VI_KEY_KP_5            ::Vi::Key::KP5
#define VI_KEY_KP_6            ::Vi::Key::KP6
#define VI_KEY_KP_7            ::Vi::Key::KP7
#define VI_KEY_KP_8            ::Vi::Key::KP8
#define VI_KEY_KP_9            ::Vi::Key::KP9
#define VI_KEY_KP_DECIMAL      ::Vi::Key::KPDecimal
#define VI_KEY_KP_DIVIDE       ::Vi::Key::KPDivide
#define VI_KEY_KP_MULTIPLY     ::Vi::Key::KPMultiply
#define VI_KEY_KP_SUBTRACT     ::Vi::Key::KPSubtract
#define VI_KEY_KP_ADD          ::Vi::Key::KPAdd
#define VI_KEY_KP_ENTER        ::Vi::Key::KPEnter
#define VI_KEY_KP_EQUAL        ::Vi::Key::KPEqual

#define VI_KEY_LEFT_SHIFT      ::Vi::Key::LeftShift
#define VI_KEY_LEFT_CONTROL    ::Vi::Key::LeftControl
#define VI_KEY_LEFT_ALT        ::Vi::Key::LeftAlt
#define VI_KEY_LEFT_SUPER      ::Vi::Key::LeftSuper
#define VI_KEY_RIGHT_SHIFT     ::Vi::Key::RightShift
#define VI_KEY_RIGHT_CONTROL   ::Vi::Key::RightControl
#define VI_KEY_RIGHT_ALT       ::Vi::Key::RightAlt
#define VI_KEY_RIGHT_SUPER     ::Vi::Key::RightSuper
#define VI_KEY_MENU            ::Vi::Key::Menu
