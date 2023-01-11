#pragma once

#ifdef VI_PLATFORM_WINDOWS

	extern Vi::Application* Vi::CreateApplication();

	int main(int argc, char** argv) {

		Vi::Log::Init();
		VI_CORE_WARN("(EntryPoint) Initialized log!");
		VI_CORE_INFO("(EntryPoint) Welcome To Me Vi!");
		auto app = Vi::CreateApplication();
		
		app->Run();
		
		delete app;
	
	}
#endif