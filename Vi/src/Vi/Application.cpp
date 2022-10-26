#include "Vipch.h"

#include "Application.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Log.h"




namespace Vi {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		VI_TRACE(e);

		
		while (true)
		{
			
		};
	}


}