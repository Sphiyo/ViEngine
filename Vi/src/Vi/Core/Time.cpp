#include "Vipch.h"
#include "Timestep.h"

#include <GLFW/glfw3.h>

namespace Vi {
	 
	float Time::GetTime()												// temp
	{
		return (float)glfwGetTime();
	}
}