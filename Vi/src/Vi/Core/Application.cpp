#include "Vipch.h"
#include "Application.h"

#include "Vi/Renderer/Renderer.h"
#include "Input.h"


namespace Vi {

	Application* Application::s_Instance = nullptr;

	Application::Application()	{
		VI_CORE_ASSERT(!s_Instance, "(Application) Application Already Exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(VI_BINDEVENT(Application::OnEvent));
		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run() {	

		while (m_Running) {
			Timestep timestep = Time::GetTime() - m_LastFrameTime;
			m_LastFrameTime = Time::GetTime();
			
			if(!m_Minimized) {
				for (Layer* layer : m_LayerStack) { layer->OnUpdate(timestep); }
			}

			//ImGui Loop
			m_ImGuiLayer->Begin();			
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			// Window Poll Event And Swap Buffers
			m_Window->OnUpdate();
		};
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VI_BINDEVENT(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(VI_BINDEVENT(Application::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(VI_BINDEVENT(Application::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VI_BINDEVENT(Application::OnMouseButtonReleased));
		
		// LookForTheLastPossibleLayerThatCanRecieveEventAndIfItGetsHandledThenBreak
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {

			(*--it)->OnEvent(e);
			if (e.Handled) { break; }
		}	
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e) {

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetHeight() == 0 || e.GetWidth() == 0) {

			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
	bool Application::OnKeyPressed(KeyPressedEvent& e) {

		if(e.GetKeyCode() == VI_KEY_ESCAPE) {

			CloseWindow();
		};
		if (e.GetKeyCode() == VI_KEY_F11) {
			
			m_Window->ToggleFullScreen();
		};

		///// To Remove ////////////////////////
		if (e.GetKeyCode() == VI_KEY_0) {

			m_Window->SetFullScreen(true,0);			
		};

		if (e.GetKeyCode() == VI_KEY_1) {

			m_Window->SetFullScreen(true, 1);
		};
		////////////////////////////////////////
	

		return false;
	}
	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		m_Window->SetCursor(CursorState::DISABLED);
		return false;
	}
	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		m_Window->SetCursor(CursorState::NORMAL);
		return false;
	}
}