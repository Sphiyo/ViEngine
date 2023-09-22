#pragma once

#include "Vi/Core/Input.h"

#include "Vi/Core/Application.h"
#include "GLFW/glfw3.h"




namespace Vi {

	class WindowsInput : public Input
	{

		virtual bool IsKeyPressedImpl(KeyCode key) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;


		virtual std::pair<float, float> GetMousePositionImpl() override;

		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;



	};

}