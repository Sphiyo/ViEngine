#include "Vipch.h"
#include "Vi/Core/Input.h"

#ifdef VI_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Vi {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef VI_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		VI_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}