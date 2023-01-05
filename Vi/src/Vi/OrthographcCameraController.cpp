#include "Vipch.h"
#include "OrthographcCameraController.h"

#include "Vi/Input.h"
#include "Vi/KeyCodes.h"

namespace Vi {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
		: m_AspectRatio(aspectRatio), m_OCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation) {

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {

		if (Input::IsKeyPressed(VI_KEY_W) || Input::IsKeyPressed(VI_KEY_UP)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}           // temp 

		if (Input::IsKeyPressed(VI_KEY_S) || Input::IsKeyPressed(VI_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}           // temp 

		if (Input::IsKeyPressed(VI_KEY_A) || Input::IsKeyPressed(VI_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}           // temp 		

		if (Input::IsKeyPressed(VI_KEY_D) || Input::IsKeyPressed(VI_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}           // temp 

		if (m_Rotation) {
			if (Input::IsKeyPressed(VI_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(VI_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			m_OCamera.SetRotation(m_CameraRotation);
		}

		m_OCamera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VI_BINDEVENT(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_OCamera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_OCamera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}