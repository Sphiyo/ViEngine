#include "Vipch.h"
#include "CameraController.h"

#include "Vi/Core/Input.h"
#include "Vi/Core/KeyCodes.h"
#include <Vi/Core/MouseButtonCodes.h>

namespace Vi {

	
	CameraController::CameraController(float aspectRatio) : m_AspectRatio(aspectRatio), m_Camera3d() {

	}

	void CameraController::OnUpdate(Timestep ts) {

		glm::vec3 locOffset(0.0f);

		if (m_MouseMiddleButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMiddleMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastMiddleMouseYPos;

			locOffset += glm::vec3(0.0f, XOffset, -YOffset);

			LastMiddleMouseXPos += XOffset;
			LastMiddleMouseYPos += YOffset;
		}

		if (m_MouseRightButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastRightMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastRightMouseYPos;

			m_Camera3d.AddLocalRotation(ts * m_CameraRotationSpeed, XOffset, YOffset);
			
			LastRightMouseXPos += XOffset;
			LastRightMouseYPos += YOffset;
		}

		if (m_MouseLeftButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastLeftMouseXPos;
			float YOffset = Vi::Input::GetMouseY() - LastLeftMouseYPos;

	//		locOffset += glm::vec3(-YOffset, 0.0f, 0.0f);
			m_Camera3d.AddForwardWorldOffset(ts * m_CameraTranslationSpeed, glm::vec3(-YOffset, 0.0f, 0.0f));
			m_Camera3d.AddLocalRotation(ts * m_CameraRotationSpeed, XOffset, 0.0f);

			LastLeftMouseXPos += XOffset;
			LastLeftMouseYPos += YOffset;
		}

		if (Input::IsKeyPressed(VI_KEY_W) || Input::IsKeyPressed(VI_KEY_UP)) {
			locOffset += glm::vec3(1.0f, 0.0f, 0.0f);
		}          

		if (Input::IsKeyPressed(VI_KEY_S) || Input::IsKeyPressed(VI_KEY_DOWN)) {
			locOffset += glm::vec3(-1.0f, 0.0f, 0.0f);
		}           

		if (Input::IsKeyPressed(VI_KEY_D) || Input::IsKeyPressed(VI_KEY_RIGHT)) {
			locOffset += glm::vec3(0.0f, 1.0f, 0.0f);
		}          

		if (Input::IsKeyPressed(VI_KEY_A) || Input::IsKeyPressed(VI_KEY_LEFT)) {
			locOffset += glm::vec3(0.0f, -1.0f, 0.0f);
		}          	
				
		if (Input::IsKeyPressed(VI_KEY_E)) {
			locOffset += glm::vec3(0.0f, 0.0f, 1.0f);
		}
		if (Input::IsKeyPressed(VI_KEY_Q)) {
			locOffset += glm::vec3(0.0f, 0.0f, -1.0f);
		}

		m_Camera3d.AddLocalOffset(ts * m_CameraTranslationSpeed, locOffset);
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