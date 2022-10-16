#pragma once

#ifdef VI_PLATFORM_WINDOWS

extern Vi::Application* Vi::CreateApplication();

int main(int argc, char** argv)
{

	printf("Welcome To Vi");
	auto app = Vi::CreateApplication();
	app->Run();
	delete app;

}

#endif