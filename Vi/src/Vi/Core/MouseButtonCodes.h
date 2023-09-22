#pragma once

namespace Vi
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define VI_MOUSE_BUTTON_0      ::Vi::Mouse::Button0
#define VI_MOUSE_BUTTON_1      ::Vi::Mouse::Button1
#define VI_MOUSE_BUTTON_2      ::Vi::Mouse::Button2
#define VI_MOUSE_BUTTON_3      ::Vi::Mouse::Button3
#define VI_MOUSE_BUTTON_4      ::Vi::Mouse::Button4
#define VI_MOUSE_BUTTON_5      ::Vi::Mouse::Button5
#define VI_MOUSE_BUTTON_6      ::Vi::Mouse::Button6
#define VI_MOUSE_BUTTON_7      ::Vi::Mouse::Button7
#define VI_MOUSE_BUTTON_LAST   ::Vi::Mouse::ButtonLast
#define VI_MOUSE_BUTTON_LEFT   ::Vi::Mouse::ButtonLeft
#define VI_MOUSE_BUTTON_RIGHT  ::Vi::Mouse::ButtonRight
#define VI_MOUSE_BUTTON_MIDDLE ::Vi::Mouse::ButtonMiddle
