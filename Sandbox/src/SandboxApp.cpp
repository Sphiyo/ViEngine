#include <Vi.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi


class ExampleLayer : public Vi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		
	}

	void OnUpdate() override
	{
		
		if(Vi::Input::IsKeyPressed(VI_KEY_TAB))
			VI_TRACE("Tab Is Pressed (Poll)");

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