#pragma once


#include "Core.h"
#include "Window.h"
#include "Vi/LayerStack.h"
#include "Events/Event.h"
#include "Vi/Events/ApplicationEvent.h"




namespace Vi {

	class VI_API Application
	{
	public:

		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		
		bool OnWindowClosed(WindowCloseEvent& e);
		
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:

		static Application* s_Instance;



	};

	// To Be Defined In Client
	Application* CreateApplication();
}