#include "Vipch.h"
#include "Application.h"

#include "Vi/Renderer/Renderer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <GLFW/glfw3.h>
#include "Vi/Physics/VPhysicsLayer.h"
//#include "Vi/Debug/Timer.h"

//#define PX_GENERATE_STATIC_LIBRARIES

namespace Vi {


	Application* Application::s_Instance = nullptr;

	Application::Application()	{

		VI_PROFILE_FUNCTION();

		VI_CORE_ASSERT(!s_Instance, "(Application) Application Already Exists!");
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(VI_BINDEVENT(Application::OnEvent));
		Renderer::Init();
	//	m_ImGuiLayer = new ImGuiLayer();
	//	PushOverlay(m_ImGuiLayer);
		m_PhysicsLayer = new Physics::VPhysicsLayer();
		PushOverlay(m_PhysicsLayer);

#if IMGUI == 1


#if IMGUITHREADED == 0
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
#endif


#if  IMGUITHREADED == 1
		m_ImGuiThread.AddFunction([&] {
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
			});
		m_ImGuiThread.Run();
#endif

#endif //  IMGUI == 1


	}

	Application::~Application() {
		VI_PROFILE_FUNCTION();
		m_PhysicsLayer->OnDetach();
		Renderer::Shutdown();

#if IMGUI == 1

#if IMGUITHREADED == 1
		m_ImGuiThread.AddFunction([&] {
			m_ImGuiLayer->OnDetach();
			});
		m_ImGuiThread.Shutdown();
#endif

#endif // IMGUI == 1
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

	void Application::PopOverlay(Layer* overlay) {
		VI_PROFILE_FUNCTION();

		m_LayerStack.PopOverlay(overlay);

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
	void Application::Run()	{
		VI_PROFILE_FUNCTION();

		const float fixedTimeStep = 1.0f / 250.0f;
		float accumulator = 0.0f;

		while (m_Running) 
		{
			VI_PROFILE_SCOPE("RunLoop");

			RendererAPI::ClearDrawCallCounts();

			Timestep timestep = Time::GetTime() - m_LastFrameTime;
			m_LastFrameTime = Time::GetTime();

			if(!m_Minimized)
			{

				VI_PROFILE_SCOPE("LayerStack OnUpdate");
				
				if (!IsRenderingPaused)
				{
					
					

					{
						VI_PROFILE_SCOPE("Renderer Prep");
						RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
						RenderCommand::Clear();
					}
				
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);


				// Begin and Update Physics 
				#define FixedTick 1		
				#if FixedTick == 1

						accumulator += timestep;

						while (accumulator >= fixedTimeStep) {
							m_PhysicsLayer->BeginPhysics(fixedTimeStep);
							layer->OnFixedUpdate(fixedTimeStep);

							accumulator -= fixedTimeStep;
						}
				#elif FixedTick == 0

						m_PhysicsLayer->BeginPhysics(timestep);
						layer->OnFixedUpdate(timestep);

				#endif // FixedTick 

					}

					m_PhysicsLayer->EndPhysics();
					
				#if IMGUI == 1

					#if IMGUITHREADED == 1


					m_ImGuiThread.AddFunction([&] {

						m_ImGuiLayer->Begin();
						{


							for (Layer* layer : m_LayerStack)																															   //
								layer->OnImGuiRender();																																	   //

						}
						m_ImGuiLayer->End();

						});

					#endif


					#if IMGUITHREADED == 0

					m_ImGuiLayer->Begin();
					{
						VI_PROFILE_SCOPE("LayerStack OnImGuiRender");

						for (Layer* layer : m_LayerStack)
							layer->OnImGuiRender();
						VI_CORE_WARN("ImGUIRender For Each LAyer In App.cpp");
					}
					m_ImGuiLayer->End();

					#endif

				#endif // IMGUI == 1

				m_Window->OnUpdate();
				}
				// Render ImGui if window is not minimized

			
			}

		};

		// Pop m_ImGuiLayer if !m_Running
		#if IMGUI == 1
#if IMGUITHREADED == 0

		PopOverlay(m_ImGuiLayer);

#endif

#if IMGUITHREADED == 1

		m_ImGuiThread.AddFunction([&] {
			PopOverlay(m_ImGuiLayer);

			});

#endif

		#endif // IMGUI == 1

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

		if (e.GetKeyCode() == VI_KEY_ESCAPE) {

			CloseWindow();
		};
		if (e.GetKeyCode() == VI_KEY_F11) {

			m_Window->ToggleFullScreen();
		};

		///// To Remove ////////////////////////
		if (e.GetKeyCode() == VI_KEY_0) {

			m_Window->SetFullScreen(true, 0);
		};

		if (e.GetKeyCode() == VI_KEY_1) {

			m_Window->SetFullScreen(true, 1);
		};
		
		if (e.GetKeyCode() == VI_KEY_V) {

			m_Window->ToggleVSync();
		};
		////////////////////////////////////////


		return false;
	}
	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e) {


		bool IsLocallyControlled = true;

#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
#endif

		if (IsLocallyControlled && !OnMouseFirstPressed) {
			m_Window->SetCursor(CursorState::DISABLED);
			OnMouseFirstPressed = true;
		}
		return false;
	}
	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		bool IsLocallyControlled = true;

#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
#endif

		if (IsLocallyControlled) {


		}
		return false;
	}
	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		OnMouseFirstPressed = false;
		bool IsLocallyControlled = true;

#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
#endif

		if (IsLocallyControlled) {

			if (Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_LEFT) || Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_RIGHT) || Input::IsMouseButtonPressed(VI_MOUSE_BUTTON_MIDDLE)) {
				//	m_Window->SetCursor(CursorState::DISABLED); 
				return false;
			}
		}
		m_Window->SetCursor(CursorState::NORMAL);
		return false;
	}
}