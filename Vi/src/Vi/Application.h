#pragma once


#include "Core.h"





namespace Vi {

	class VI_API Application
	{
	public:

		Application();

		virtual ~Application();

		void Run();


	};

	// To Be Defined In Client
	Application* CreateApplication();
}