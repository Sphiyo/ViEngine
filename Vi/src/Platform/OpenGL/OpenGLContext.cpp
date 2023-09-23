#include "Vipch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Vi {



	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {

		VI_CORE_ASSERT(windowHandle, "(OpenGLContext) Window handle is NULL");
	}

	void OpenGLContext::Init() {
		VI_PROFILE_FUNCTION();

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

#ifdef VI_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		VI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 3), "Vi requires at least OpenGL version 4.3!");
#endif
	}

	void OpenGLContext::SwapBuffers() {	
		VI_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}