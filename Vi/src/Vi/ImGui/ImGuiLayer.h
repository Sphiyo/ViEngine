#pragma once
#include "Vi/Layer.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"


namespace Vi {



	class VI_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
	
		float m_Time = 0.0f;
	
	};


}
