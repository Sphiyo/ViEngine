#pragma once
#include "Core.h"
#include "Window.h"
#include "Vi/LayerStack.h"
#include "Events/Event.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Core/Timestep.h"
#include "Vi/ImGui/ImGuiLayer.h"

namespace Vi {

	class Application {

	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		inline void CloseWindow() { m_Running = false; }
	
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		
	private:
		static Application* s_Instance;
	
	};

	// To Be Defined In Client
	Application* CreateApplication();

}