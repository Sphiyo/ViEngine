#include <Vi.h>

class ExampleLayer : public Vi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		VI_INFO("ExampleLayer::Update");
	}

	void OnEvent(Vi::Event& event) override
	{
		VI_TRACE("{0}", event);
	}
};

class Sandbox : public Vi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Vi::ImGuiLayer());
	}
	~Sandbox()
	{
		
	}
};


Vi::Application* Vi::CreateApplication()
{
	return new Sandbox();
}