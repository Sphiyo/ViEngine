#pragma once
#include "Vi/Renderer/OrthographicCamera.h"
#include "Vi/Core/Timestep.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/MouseEvent.h"

namespace Vi {

	class OrthographicCameraController {

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_OCamera; }
		const OrthographicCamera& GetCamera() const { return m_OCamera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_OCamera;
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 2.0f, m_CameraRotationSpeed = 90.0f;
	};




}