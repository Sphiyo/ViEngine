#pragma once
#include "Vi/Renderer/Camera.h"
#include "Vi/Core/Timestep.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"

namespace Vi {

	class CameraController {

	public:
		CameraController();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	//	OrthographicCamera& GetCamera() { return m_Camera; }
	//	const OrthographicCamera& GetCamera() const { return m_Camera; }


		Camera& GetCamera3D() { return m_Camera3d; }
		const Camera& GetCamera3D() const { return m_Camera3d; }


		
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 45.0f, m_MouseXSensitivity = 5.0f, m_MouseYSensitivity = 5.0f;


		Camera m_Camera3d;

		float LastLeftMouseXPos = 0.0f;
		float LastLeftMouseYPos = 0.0f;
		
		float LastRightMouseXPos = 0.0f;
		float LastRightMouseYPos = 0.0f;
		
		float LastMiddleMouseXPos = 0.0f;
		float LastMiddleMouseYPos = 0.0f;
		
		bool m_MouseRightButtonPressed = false;
		bool m_MouseLeftButtonPressed = false;
		bool m_MouseMiddleButtonPressed = false;
		float m_FOVCache = 90.0;

		int m_BoostFactor = 1;
	};




}