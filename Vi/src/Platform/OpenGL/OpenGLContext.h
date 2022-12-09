#pragma once

#include "Vi/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Vi {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
		float x = 0.0f;
		float y = 1.0f;
		bool Addx = true;
		bool Addy = true;
	};

}