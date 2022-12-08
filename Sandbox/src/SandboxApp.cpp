#include <Vi.h>

#include "imgui/imgui.h"

class ExampleLayer : public Vi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		
	}

	virtual void OnUpdate() override
	{
		
		if(Vi::Input::IsKeyPressed(VI_KEY_TAB))
			VI_TRACE("Tab Is Pressed (Poll)");

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Vi::Event& event) override
	{
		if (event.GetEventType() == Vi::EventType::KeyPressed)
		{
			Vi::KeyPressedEvent& e = (Vi::KeyPressedEvent&)event;
			if(e.GetKeyCode() == VI_KEY_TAB){ VI_TRACE("Tab Is Pressed (Event)"); }
			VI_TRACE("{0}", (char)e.GetKeyCode());
		}
		
	}
};

class Sandbox : public Vi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
		
	}
};


Vi::Application* Vi::CreateApplication()
{
	return new Sandbox();
}