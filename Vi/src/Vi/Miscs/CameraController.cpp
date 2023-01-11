#include "Vipch.h"
#include "CameraController.h"

#include "Vi/Core/Input.h"
#include "Vi/Core/KeyCodes.h"
#include <Vi/Core/MouseButtonCodes.h>

namespace Vi {

	
	CameraController::CameraController(float aspectRatio) : m_AspectRatio(aspectRatio), m_Camera3d(glm::vec3(-3.0f,1.0f,15.0f), glm::vec3(15.0f, 70.0f, 0.0f)) {
		
	}

	void CameraController::OnUpdate(Timestep ts) {

		glm::vec3 locationOffset(0.0f);
	//	glm::vec3 rotationOffset(0.0f);

		if (m_MouseMiddleButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMiddleMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastMiddleMouseYPos;

			locationOffset += glm::vec3(0.0f, XOffset * m_MouseXSensitivity * m_CameraTranslationSpeed, -YOffset *  m_MouseYSensitivity * m_CameraTranslationSpeed);

			LastMiddleMouseXPos += XOffset;
			LastMiddleMouseYPos += YOffset;
		}

		if (m_MouseRightButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastRightMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastRightMouseYPos;

			m_Camera3d.AddLocalRotation(ts, XOffset * m_MouseXSensitivity * m_CameraRotationSpeed, YOffset * m_MouseYSensitivity * m_CameraRotationSpeed);
	//		rotationOffset += glm::vec3(XOffset * m_MouseXSensitivity, YOffset * m_MouseYSensitivity, 0.0f);

			LastRightMouseXPos += XOffset;
			LastRightMouseYPos += YOffset;
		}

		if (m_MouseLeftButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastLeftMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastLeftMouseYPos;

	//		locationOffset += glm::vec3(-YOffset * m_CameraTranslationSpeed * m_MouseYSensitivity, 0.0f, 0.0f);
			m_Camera3d.AddForwardWorldOffset(ts, glm::vec3(-YOffset * m_CameraTranslationSpeed * m_MouseYSensitivity, 0.0f, 0.0f));
			m_Camera3d.AddLocalRotation(ts , XOffset * m_MouseXSensitivity * m_CameraRotationSpeed, 0.0f);

			LastLeftMouseXPos += XOffset;
			LastLeftMouseYPos += YOffset;
		}

		if (Input::IsKeyPressed(VI_KEY_W) || Input::IsKeyPressed(VI_KEY_UP)) {
			locationOffset += glm::vec3(1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed);
		}          

		if (Input::IsKeyPressed(VI_KEY_S) || Input::IsKeyPressed(VI_KEY_DOWN)) {
			locationOffset += glm::vec3(-1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed);
		}           

		if (Input::IsKeyPressed(VI_KEY_D) || Input::IsKeyPressed(VI_KEY_RIGHT)) {
			locationOffset += glm::vec3(0.0f, 1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed);
		}          

		if (Input::IsKeyPressed(VI_KEY_A) || Input::IsKeyPressed(VI_KEY_LEFT)) {
			locationOffset += glm::vec3(0.0f, -1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed);
		}          	
				
		if (Input::IsKeyPressed(VI_KEY_E)) {
			locationOffset += glm::vec3(0.0f, 0.0f, 1.0f) * glm::vec3(m_CameraTranslationSpeed);
		}
		if (Input::IsKeyPressed(VI_KEY_Q)) {
			locationOffset += glm::vec3(0.0f, 0.0f, -1.0f) * glm::vec3(m_CameraTranslationSpeed);
		}

		m_Camera3d.AddLocalOffset(ts , locationOffset);
	//	m_Camera.SetWorldRotation(m_CameraRotation);

		

	//	m_Camera.SetWorldLocation(m_CameraPosition);

	//	m_CameraTranslationSpeed = m_ZoomLevel;

	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VI_BINDEVENT(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(VI_BINDEVENT(CameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VI_BINDEVENT(CameraController::OnMouseButtonReleased));

																							 // temp
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (!m_MouseRightButtonPressed) {
			m_Camera3d.AddLocalOffset(1.0f, glm::vec3(e.GetYOffset(), 0.0, 0.0));
		}
		else {
			
			float fovOffset = - e.GetYOffset() * 5.0f;
			
			m_Camera3d.AddFOV(fovOffset);
		}
		VI_CORE_INFO("Fov : {}", m_Camera3d.GetFOV());
	//	m_ZoomLevel -= e.GetYOffset() * 0.25f;
	//	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	//	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		
		
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE) {

			LastMiddleMouseXPos = Vi::Input::GetMousePosition().first;
			LastMiddleMouseYPos = Vi::Input::GetMousePosition().second;
			m_MouseMiddleButtonPressed = true;
		}
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_RIGHT) {

			LastRightMouseXPos = Vi::Input::GetMousePosition().first;
			LastRightMouseYPos = Vi::Input::GetMousePosition().second;
			m_MouseRightButtonPressed = true;

			m_FOVCache = m_Camera3d.GetFOV();
		}
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_LEFT) {

			LastLeftMouseXPos = Vi::Input::GetMousePosition().first;
			LastLeftMouseYPos = Vi::Input::GetMousePosition().second;
			m_MouseLeftButtonPressed = true;
		}
		
		return false;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {

		if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE) { m_MouseMiddleButtonPressed = false; }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_RIGHT) { m_MouseRightButtonPressed = false;		m_Camera3d.SetFOV(m_FOVCache); }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_LEFT) { m_MouseLeftButtonPressed = false; }

		return false;
	}




	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	//	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}