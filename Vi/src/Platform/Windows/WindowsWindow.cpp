#include "Vipch.h"
#include "WindowsWindow.h"

#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"		

// test
#include "Vi/Core/Input.h"
#include "Vi/Core/KeyCodes.h"										// temp 
#include "Vi/Core/Application.h"


namespace Vi {

	static bool s_GLFWInitialized = false;

	static void GLFWErroeCallback(int error_code, const char* description) {

		VI_CORE_ERROR("GLFW Error ({0}):({1})", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
	
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{	
		Shutdown();
	}



	
	void WindowsWindow::Init(const WindowProps& props)
	{


		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			//todo glfwterminate on system shutdown
			int success = glfwInit();
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			//glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
			//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
			//glfwWindowHint(GLFW_CENTER_CURSOR , GLFW_TRUE); // Doesn't Work On Windowed Mode
			//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // GLFW_DONT_CARE For Unlimited
			VI_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErroeCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr/*glfwGetPrimaryMonitor() // ForFullScreen*/, nullptr);
		
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		
		//Set GLFW Callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			data.Width = width;
			data.Height = height;
			
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});	
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {  
			
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);

		});		
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}

				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}

			}
	
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	
			KeyTypedEvent event(key);
			data.EventCallback(event);
		
		});				
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		
			switch (action)
			{

				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}

			}
			
		});		
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		

		});		
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);

		});		

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}

	void WindowsWindow::OnUpdate()
	{
		//test																						// temp 
		if (Input::IsKeyPressed(VI_KEY_ESCAPE))
			{ Application::Get().CloseWindow(); };

		if (Input::IsKeyPressed(VI_KEY_F11)) {
			if(glfwGetWindowMonitor(m_Window) == NULL)
				glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
			else if(glfwGetWindowMonitor(m_Window) != NULL)
				glfwSetWindowMonitor(m_Window, NULL, 0, 0, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
		};
		//
		glfwPollEvents();
		m_Context->SwapBuffers();
		
		
		//myImplementation Closed = glfwWindowShouldClose(m_Window);										 
	}

	void WindowsWindow::SetVSync(bool enabled)
	{

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	
	}

}
