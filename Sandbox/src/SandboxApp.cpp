#include <Vi.h>

class Sandbox : public Vi::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};


Vi::Application* Vi::CreateApplication()
{
	return new Sandbox();
}