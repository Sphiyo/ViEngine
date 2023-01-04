#pragma once

#ifdef VI_PLATFORM_WINDOWS

	extern Vi::Application* Vi::CreateApplication();

	int main(int argc, char** argv) {

		Vi::Log::Init();
		VI_CORE_WARN("Initialized log!");
		VI_INFO("Welcome To Me Vi! ");
		auto app = Vi::CreateApplication();
		
		app->Run();
		
		delete app;
	
	}
#endif