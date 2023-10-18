#pragma once

#define IMGUI 1
#define IMGUITHREADED 0

#include "Vi/Core/Layer.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"
#if IMGUI == 1


struct GLFWwindow;




namespace Vi {


#if IMGUITHREADED == 1

	class VI_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		//virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		// Dark or Light
		virtual void SetImGuiTheme(bool bStyleDark);

		void JoinThread();

	public:

		bool m_CapturingKeyboard = false;
		bool m_CapturingMouse = false;


		bool m_ShouldAttach = false;
		bool m_ShouldRun = false;
		bool m_ShouldDetach = false;
		bool m_AttachedSuccess = false;
		bool m_DettachedSuccess = false;
		std::thread m_LayerThread;
		GLFWwindow* m_ImGuiWindow;
	private:

		float m_Time = 0.0f;

		bool m_DarkImgui = true;
	};

#elif IMGUITHREADED == 0

	class VI_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		// Dark or Light
		virtual void SetImGuiTheme(bool bStyleDark);

	public:

		bool m_CapturingKeyboard = false;
		bool m_CapturingMouse = false;
		
	private:

		float m_Time = 0.0f;

		bool m_DarkImgui = true;
	};


#endif


}

#endif