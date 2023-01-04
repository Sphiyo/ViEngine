#include "Vipch.h"
#include "Timestep.h"

#include <GLFW/glfw3.h>

namespace Vi {
	 
#ifdef VI_PLATFORM_WINDOWS
	float Time::GetTime()												// temp
	{
		return (float)glfwGetTime();
	}
#else 
	float Time::GetTime()
	{
		VI_CORE_ASSERT(0, " GetTime Only Defined For Windows, You Can Define It In Your Platform !");
		return -1;
	}
#endif
}