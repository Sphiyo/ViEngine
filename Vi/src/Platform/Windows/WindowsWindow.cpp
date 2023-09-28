#include "Vipch.h"
#include "WindowsWindow.h"

#include "Vi/Core/Input.h"

#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"

#include "Vi/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"		




namespace Vi {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErroeCallback(int error_code, const char* description) {

		VI_CORE_ERROR("(WindowsWindow) GLFW Error ({0}):({1})", error_code, description);
	}

	/*Window* Window::Create(const WindowProps& props) {

		return new WindowsWindow(props);
	}*/

	WindowsWindow::WindowsWindow(const WindowProps& props) {

		VI_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		VI_PROFILE_FUNCTION();

		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProps& props) {

		VI_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VI_CORE_INFO("(WindowsWindow) Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0) {
			
			VI_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			//glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
			//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
			//glfwWindowHint(GLFW_CENTER_CURSOR , GLFW_TRUE); // Doesn't Work On Windowed Mode
			//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // GLFW_DONT_CARE For Unlimited
		//	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			VI_CORE_ASSERT(success, "(WindowsWindow) Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErroeCallback);
			
		}
		{
			VI_PROFILE_SCOPE("glfwCreateWindow");
	#if defined(VI_DEBUG)
			if(Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif 

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr/*glfwGetPrimaryMonitor() // ForFullScreen*/, nullptr);
			++s_GLFWWindowCount;
		}

	//	glfwDefaultWindowHints();
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		////////
		glfwSetWindowPos(m_Window, (int)props.Width / 4, (int)props.Height/4 ); // test
	//	glfwShowWindow(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetWindowAspectRatio(m_Data.m_AspectRatio[0], m_Data.m_AspectRatio[1]); // temp
		SetVSync(true);
		CheckMonitors();



		SetCursor(CursorState::NORMAL);
		
		//Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<KeyCode>(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(static_cast<KeyCode>(keycode));
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(static_cast<MouseCode>(button));
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});

	}

	void WindowsWindow::Shutdown() {
		VI_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void* WindowsWindow::GetNativeWindow() const {
	
		return m_Window;
	}

	void WindowsWindow::OnUpdate() {
		VI_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
											 
	}

	bool WindowsWindow::IsFullScreen() const {

		return m_Data.FullScreen;
	}
	bool& WindowsWindow::IsFullScreenRef() {

		return m_Data.FullScreen;
	}

	void WindowsWindow::ToggleFullScreen()
	{
		if (!IsFullScreen()) {
			SetFullScreen(true);
			return;
		}
			
		else
			SetFullScreen(false);
	}

	
	void WindowsWindow::SetFullScreen(bool enable, int16_t monitorIndex) {
		
		CheckMonitors();
		int lastMonitorIndex = m_Data.m_MonitorsCount - 1;

		if (monitorIndex < 0) {

			monitorIndex = m_Data.m_ActiveMonitor;
			VI_CORE_INFO(" (WindowsWindow) Set Monitor Index To The Last Active Monitor");
		}

				
		if (monitorIndex > lastMonitorIndex) {

			VI_CORE_WARN(" (WindowsWindow) Monitor Index : {} Not Found.", monitorIndex);
			VI_CORE_WARN(" (WindowsWindow) The Last Monitor Index Is : {}.", lastMonitorIndex);
			monitorIndex = lastMonitorIndex;
			VI_CORE_WARN(" (WindowsWindow) Set Monitor Index To Last Index : {}.", lastMonitorIndex);
		}

		if (enable) {

			m_Data.m_ActiveMonitor = monitorIndex;
			VI_CORE_INFO(" (WindowsWindow) Set FullScreen Mode, Active Monitor Index Is : {}", m_Data.m_ActiveMonitor);
			glfwSetWindowMonitor(m_Window, m_Data.m_Monitors[m_Data.m_ActiveMonitor], 0, 0, m_Data.Width, m_Data.Height, 30);
		}

		else {

			VI_CORE_INFO(" (WindowsWindow) Set Windowed Mode, Last Active Monitor Index Is : {}", m_Data.m_ActiveMonitor);
			glfwSetWindowMonitor(m_Window, NULL, m_Data.Width / 4, m_Data.Height / 4, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
		}
		
		m_Data.FullScreen = enable;
	}

	void WindowsWindow::SetResolution(int width, int height) {

		glfwSetWindowSize(m_Window, width, height);
		m_Data.Width = width;
		m_Data.Height = height;
	}

	void WindowsWindow::SetWindowAspectRatio(int number, int denominator) {

		glfwSetWindowAspectRatio(m_Window, number, denominator);
	}

	void WindowsWindow::CheckMonitors()
	{
		/////////// Check For Monitors Count ///////////////////
		int monitorCount = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		////////////////////////////////////////////////////////

		/////////// Set Monitors Count ///////
		m_Data.m_MonitorsCount = monitorCount;
		//////////////////////////////////////

		/////////// Set Vector Array //////////////////////////////////////////////
		if (m_Data.m_Monitors.size() == monitorCount) { return; }

		else {
			m_Data.m_Monitors.clear();

			for (int i = 0; i < monitorCount; i++) {
				
				m_Data.m_Monitors.push_back(monitors[i]);
			}
			

			VI_CORE_INFO("(WindowsWindow) {} : Monitors Connected", monitorCount);
		}
		///////////////////////////////////////////////////////////////////////////
	}


	void WindowsWindow::SetVSync(bool enabled) {
		VI_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}
	bool WindowsWindow::ToggleVSync() {

		VI_PROFILE_FUNCTION();

		if (m_Data.VSync) {

			glfwSwapInterval(0);
			m_Data.VSync = false;
		}
		else {

			glfwSwapInterval(1);
			m_Data.VSync = true;
		}
		return m_Data.VSync;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;	
	}
		

	void WindowsWindow::SetCursor(CursorState cursorState, bool center) {

		
		if (center) { glfwSetCursorPos(m_Window, (double)(m_Data.Width / 2.0), (double)(m_Data.Height / 2.0)); }
		switch (cursorState)
		{
		case Vi::CursorState::NORMAL:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);			
			break;
		case Vi::CursorState::HIDDEN:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			break;
		case Vi::CursorState::DISABLED:			
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case Vi::CursorState::UNAVAILABLE:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);
			break;

		default:
			break;
		}

		m_CursorState = cursorState;
	}

	void WindowsWindow::ToggleCursor() {

		switch (m_CursorState)
		{
		case CursorState::NORMAL:
			SetCursor(CursorState::DISABLED);
			break;
		case CursorState::HIDDEN:
			SetCursor(CursorState::NORMAL);
			break;
		case CursorState::DISABLED:
			SetCursor(CursorState::NORMAL);
			break;
		case CursorState::UNAVAILABLE:
			SetCursor(CursorState::UNAVAILABLE);
			break;
		default:
			VI_CORE_ASSERT(false, "(WindowsWindow) Cursor State Unknown! ");
			break;
		}		
	}

	void WindowsWindow::SetCursorPosition(double x, double y)	{
		
	//	int width, height;
	//	glfwGetWindowSize(m_Window, &width, &height);
	//	VI_CORE_INFO("width {0} height {1}", width, height);
	//	
	//	glfwGetWindowPos(m_Window, &width, &height);
	//	VI_CORE_INFO("xpos {0} ypos {1}", width, height);
	//	
	//	double xpos, ypos;
	//	glfwGetCursorPos(m_Window, &xpos, &ypos);
	//	VI_CORE_INFO("xpos {0} ypos {1}", xpos, ypos);
	//	
	//	GLFWmonitor* monitor = glfwGetWindowMonitor(m_Window);
	//	if (monitor) {
	//		glfwGetMonitorPos(monitor, &width, &height);
	//		VI_CORE_INFO("xpos {0} ypos {1}", width, height);
	//	}
	//
	//	
	//	//	glfwSetCursorPos(m_Window, x, y);
	}
	
}
