#include "Vipch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Vi {



	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VI_CORE_ASSERT(windowHandle, "(OpenGLContext) Window handle is NULL");
	}

	void OpenGLContext::Init() {

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VI_CORE_ASSERT(status, "(OpenGLContext) Failed to initialize Glad");
		auto lVendor = (char*)glGetString(GL_VENDOR);
		auto lRenderer = (char*)glGetString(GL_RENDERER);
		auto lVersion = (char*)glGetString(GL_VERSION);
		VI_CORE_INFO("(OpenGLContext) OpenGL Info: ");
		VI_CORE_INFO("(OpenGLContext) OpenGL Vendor  : {0}", lVendor);
		VI_CORE_INFO("(OpenGLContext) OpenGL Renderer: {0}", lRenderer);
		VI_CORE_INFO("(OpenGLContext) OpenGL Version : {0}", lVersion);
		//glViewport(0, 450, 800, 450);
	}

	void OpenGLContext::SwapBuffers() {	
		glfwSwapBuffers(m_WindowHandle);
	}

}