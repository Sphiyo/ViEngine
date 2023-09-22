#include "Vipch.h"
#include "Vi/Core/Window.h"

#ifdef VI_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Vi
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef VI_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		VI_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}