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
		bool OnMouseMoved(MouseMovedEvent& e);

	private:

		Camera m_Camera;

		float m_CameraTranslationSpeed = 100.0f, m_CameraRotationSpeed = 180.0f;
		float m_MouseXSensitivityLook = 1.0f, m_MouseYSensitivityLook = 1.0f;
		float m_MouseXSensitivityMove = 0.01f, m_MouseYSensitivityMove = 0.01f;
		float m_MouseScrollSensitivityMove = 0.05f, m_MouseScrollSensitivityFov = 5.0f;
		
		int m_BoostFactor = 100;

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