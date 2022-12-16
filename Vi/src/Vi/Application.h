#pragma once


#include "Core.h"
#include "Window.h"
#include "Vi/LayerStack.h"
#include "Events/Event.h"
#include "Vi/Events/ApplicationEvent.h"

#include "Vi/ImGui/ImGuiLayer.h"

// temp
#include "Vi/Renderer/Shader.h"
#include "Vi/Renderer/Buffer.h"



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
		// test
		inline void CloseWindow() { m_Running = false; }
		///
	private:
		
		bool OnWindowClosed(WindowCloseEvent& e);
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;


	private:

		static Application* s_Instance;



	};

	// To Be Defined In Client
	Application* CreateApplication();
}