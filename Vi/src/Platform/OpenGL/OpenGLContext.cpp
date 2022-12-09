#include "Vipch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Vi {



	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VI_CORE_ASSERT(windowHandle, "Window handle is NULL");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VI_CORE_ASSERT(status, " Failed to initialize Glad");
		auto lVendor = (char*)glGetString(GL_VENDOR);
		auto lRenderer = (char*)glGetString(GL_RENDERER);
		auto lVersion = (char*)glGetString(GL_VERSION);
		VI_CORE_INFO("OpenGL Info: ");
		VI_CORE_INFO("OpenGL Vendor  : {0}", lVendor);
		VI_CORE_INFO("OpenGL Renderer: {0}", lRenderer);
		VI_CORE_INFO("OpenGL Version : {0}", lVersion);

		//glViewport(0, 450, 800, 450);
	}

	void OpenGLContext::SwapBuffers()
	{
	
		
	/*	glBegin(GL_TRIANGLES);
		glColor4f(0.1f, 1, 1,0.2f);
		glVertex2f(x-0.5f, y-0.5f);
		glVertex2f(x, y+0.5f);
		glVertex2f(x+0.5f, y-0.5f);
		glEnd();
		if (x <= -1.0f) { Addx = true; }
		if (x >= 1.0f) { Addx = false; }

		if (y <= -1.0f) { Addy = true; }
		if (y >= 1.0f) { Addy = false; }
		(Addx) ? (x += 0.1f) : (x -= 0.1f);
		(Addy) ? (y += 0.1f) : (y -= 0.1f);*/

		glfwSwapBuffers(m_WindowHandle);

	}

}