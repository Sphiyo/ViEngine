#pragma once
#include "Vi/Renderer/Camera.h"
#include "Vi/Core/Timestep.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"

namespace Vi {

	class CameraController {

	public:
		CameraController(const glm::vec3& worldLocation = { 0.0f,0.0f,0.0f }, const glm::vec3& worldRotation = { 0.0f,0.0f,0.0f });

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

	private:

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:

		Camera m_Camera;

		float m_CameraTranslationSpeed = 0.005f, m_CameraRotationSpeed = 0.1f, m_MouseXSensitivity = 1.0f, m_MouseYSensitivity = 1.0f;
		int m_BoostFactor = 1;

		float m_FOVCache = 90.0;

		float LastLeftMouseXPos = 0.0f;
		float LastLeftMouseYPos = 0.0f;
		
		float LastRightMouseXPos = 0.0f;
		float LastRightMouseYPos = 0.0f;
		
		float LastMiddleMouseXPos = 0.0f;
		float LastMiddleMouseYPos = 0.0f;
		
		bool m_MouseRightButtonPressed = false;
		bool m_MouseLeftButtonPressed = false;
		bool m_MouseMiddleButtonPressed = false;
	};




}