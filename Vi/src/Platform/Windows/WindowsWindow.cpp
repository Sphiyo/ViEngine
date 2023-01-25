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

		VI_CORE_ERROR("(WindowsWindow) GLFW Error ({0}):({1})", error_code, description);
	}

	Window* Window::Create(const WindowProps& props) {

		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {

		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProps& props) {

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VI_CORE_INFO("(WindowsWindow) Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			//todo glfwterminate on system shutdown
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
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr/*glfwGetPrimaryMonitor() // ForFullScreen*/, nullptr);
	//	glfwDefaultWindowHints();
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		////////
		glfwSetWindowPos(m_Window, (int)props.Width / 4, (int)props.Height/4 );
	//	glfwShowWindow(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		CheckMonitors();
//		{
//			int monitorCount = 0;
//			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
//			
//			for (int i = 0; i < monitorCount; i++) {
//
//				m_Data.m_Monitors.push_back(monitors[i]);
//			}
//			VI_CORE_INFO("(WindowsWindow) {} : Monitors Connected", monitorCount);
//			
//		}
		SetCursor(CursorState::NORMAL);

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

	void WindowsWindow::Shutdown() {

		glfwDestroyWindow(m_Window);
	}

	void* WindowsWindow::GetNativeWindow() const {
	
		return m_Window;
	}

	void WindowsWindow::OnUpdate() {

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
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
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
	
}
