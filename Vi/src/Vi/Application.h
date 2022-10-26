#pragma once


#include "Core.h"
#include "Events/Event.h"
#include "Window.h"





namespace Vi {

	class VI_API Application
	{
	public:

		Application();

		virtual ~Application();

		void Run();

	private:

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To Be Defined In Client
	Application* CreateApplication();
}