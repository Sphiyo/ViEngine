#pragma once
#include "Vi/Core/Core.h" 

#ifdef VI_PLATFORM_WINDOWS

	extern Vi::Application* Vi::CreateApplication();

	int main(int argc, char** argv) {
		 

		Vi::Log::Init();
		
		VI_PROFILE_BEGIN_SESSION("Startup","ViProfile-Startup.json");
		auto app = Vi::CreateApplication();
		VI_PROFILE_END_SESSION();

		VI_PROFILE_BEGIN_SESSION("Runtime", "ViProfile-Runtime.json");
	//	app->RunGUIAsync();
		app->Run();
		VI_PROFILE_END_SESSION();

		VI_PROFILE_BEGIN_SESSION("Shutdown", "ViProfile-Shutdown.json");
		delete app;
		VI_PROFILE_END_SESSION();

	}
#endif