#pragma once

#ifdef VI_PLATFORM_WINDOWS

extern Vi::Application* Vi::CreateApplication();

int main(int argc, char** argv)
{
	Vi::Log::Init();
	VI_CORE_WARN("Initialized log!");
	int a = 10;
	VI_INFO("Hii! Var={0}" , a);

	 
	auto app = Vi::CreateApplication();
	app->Run();
	delete app;

}

#endif