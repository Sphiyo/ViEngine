#pragma once
#include "Vi/Core/Layer.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"



namespace Vi {



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

	



}
