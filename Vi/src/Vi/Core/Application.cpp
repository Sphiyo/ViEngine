#include "Vipch.h"
#include "Application.h"

#include "Vi/Renderer/Renderer.h"



namespace Vi {

	Application* Application::s_Instance = nullptr;

	Application::Application()	{

		VI_PROFILE_FUNCTION();

		VI_CORE_ASSERT(!s_Instance, "(Application) Application Already Exists!");
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(VI_BINDEVENT(Application::OnEvent));
		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
		VI_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		VI_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		VI_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	void Application::OnEvent(Event& e) {
		VI_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VI_BINDEVENT(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(VI_BINDEVENT(Application::OnKeyPressed));							 // temp (move to editor)
		dispatcher.Dispatch<MouseButtonPressedEvent>(VI_BINDEVENT(Application::OnMouseButtonPressed));			 // temp (move to editor)
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VI_BINDEVENT(Application::OnMouseButtonReleased));		 // temp (move to editor)
		dispatcher.Dispatch<MouseMovedEvent>(VI_BINDEVENT(Application::OnMouseMoved));							 // temp (move to editor)
		
		// LookForTheLastPossibleLayerThatCanRecieveEventAndIfItGetsHandledThenBreak
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
	void Application::Run() {	
		VI_PROFILE_FUNCTION();

		while (m_Running) {
			
			VI_PROFILE_SCOPE("RunLoop");

			Timestep timestep = Time::GetTime() - m_LastFrameTime;
			m_LastFrameTime = Time::GetTime();
			
			if(!m_Minimized) {

				VI_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack) 
					layer->OnUpdate(timestep); 
				
			}

			
			m_ImGuiLayer->Begin();	
			{
				VI_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack) 
					layer->OnImGuiRender();
				
			}
			m_ImGuiLayer->End();

		
			m_Window->OnUpdate();
		};
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e) {

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		VI_PROFILE_FUNCTION();

		if (e.GetHeight() == 0 || e.GetWidth() == 0) {

			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		Application::Get().GetWindow().SetWindowAspectRatio(16, 9);

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
		if (!m_ImGuiLayer->m_CapturingMouse) {

			m_Window->SetCursor(CursorState::DISABLED);
		}
		return false;
	}
	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_ImGuiLayer->m_CapturingMouse) {

			
		}
		return false;
	}
	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (!m_ImGuiLayer->m_CapturingMouse) { 

			if (Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_LEFT) || Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_RIGHT) || Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_MIDDLE)) {
				//	m_Window->SetCursor(CursorState::DISABLED); 
				return false;
			}
		}
		m_Window->SetCursor(CursorState::NORMAL);
		return false;
	}
}