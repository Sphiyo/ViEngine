#pragma once
#include "Core.h"
#include "Window.h"
#include "Vi/Core/LayerStack.h"
#include "Vi/Events/Event.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Core/Timestep.h"
#include "Vi/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Vi {

	class Application {

	public:

		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
		inline Window& GetWindow() { return *m_Window; }
		inline LayerStack& GetLayerStack() { return m_LayerStack; }
		inline static Application& Get() { return *s_Instance; }
		inline bool IsApplicationRunning() { return m_Running; }
		inline void CloseWindow() { m_Running = false; }
		inline ImGuiLayer* GetImGuiLayer()const { return m_ImGuiLayer; }
	private:

		void Run();
		void RunGUIAsync();
		void RunGUISync();
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

		
		
	private:

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		
	private:

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

		struct ProfileResult {

			const char* Name;
			float Time;
		};


		std::vector<ProfileResult> m_ProfileResults;

		// experimental // test // alpha
		std::thread ImGuiThread;
	};

	// To Be Defined In Client
	Application* CreateApplication();

}