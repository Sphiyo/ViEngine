#include "Vipch.h"
#include "CameraController.h"

#include "Vi/Core/Input.h"
#include "Vi/Core/KeyCodes.h"
#include <Vi/Core/MouseButtonCodes.h>

#include "Vi/Core/Application.h"

namespace Vi {

	
	CameraController::CameraController(const glm::vec3& worldLocation, const glm::vec3& worldRotation)		
										: m_Camera(glm::vec3(worldLocation), glm::vec3(worldRotation))	{
		
	}


	void CameraController::OnUpdate(Timestep ts) {
		VI_PROFILE_FUNCTION();

	//	VI_CORE_INFO("(CameraControllerCpp) TimeStep = {}", ts);
		glm::vec3 locationOffsetWorld(0.0f);
		glm::vec3 locationOffsetLocal(0.0f);
		glm::vec3 rotationOffsetLocal(0.0f);

		if (m_MouseMiddleButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMiddleMouseXPos;
			float YOffset = LastMiddleMouseYPos - Vi::Input::GetMouseY();
			locationOffsetWorld += glm::vec3(0.0f, 0.0f, YOffset * m_CameraTranslationSpeed * m_MouseYSensitivityMove);
			locationOffsetLocal += glm::vec3(0.0f, XOffset * m_MouseXSensitivityMove * m_CameraTranslationSpeed, 0.0f);

			LastMiddleMouseXPos = Vi::Input::GetMouseX();
			LastMiddleMouseYPos = Vi::Input::GetMouseY();
		}

		if (m_MouseRightButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastRightMouseXPos;
			float YOffset = LastRightMouseYPos - Vi::Input::GetMouseY();

			rotationOffsetLocal += glm::vec3(XOffset * m_MouseXSensitivityLook * m_CameraRotationSpeed, -YOffset * m_MouseYSensitivityLook * m_CameraRotationSpeed, 0.0f);

			LastRightMouseXPos = Vi::Input::GetMouseX();
			LastRightMouseYPos = Vi::Input::GetMouseY();
		}

		if (m_MouseLeftButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastLeftMouseXPos;
			float YOffset = LastLeftMouseYPos - Vi::Input::GetMouseY();
			
			locationOffsetWorld += glm::vec3(YOffset * m_CameraTranslationSpeed * m_MouseYSensitivityMove, 0.0f, 0.0f);					
			rotationOffsetLocal += glm::vec3(XOffset * m_MouseXSensitivityLook * m_CameraRotationSpeed, 0.0f, 0.0f);

			LastLeftMouseXPos  = Vi::Input::GetMouseX();
			LastLeftMouseYPos  = Vi::Input::GetMouseY();
		}
		if (true /*!Application::Get().GetImGuiLayer()->m_CapturingKeyboard*/) {

			if (Input::IsKeyPressed(VI_KEY_W) || Input::IsKeyPressed(VI_KEY_UP)) {
				locationOffsetLocal += glm::vec3(1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}

			if (Input::IsKeyPressed(VI_KEY_S) || Input::IsKeyPressed(VI_KEY_DOWN)) {
				locationOffsetLocal += glm::vec3(-1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}

			if (Input::IsKeyPressed(VI_KEY_D) || Input::IsKeyPressed(VI_KEY_RIGHT)) {
				locationOffsetLocal += glm::vec3(0.0f, 1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}

			if (Input::IsKeyPressed(VI_KEY_A) || Input::IsKeyPressed(VI_KEY_LEFT)) {
				locationOffsetLocal += glm::vec3(0.0f, -1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}

			if (Input::IsKeyPressed(VI_KEY_E)) {
				locationOffsetLocal += glm::vec3(0.0f, 0.0f, 1.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}
			if (Input::IsKeyPressed(VI_KEY_Q)) {
				locationOffsetLocal += glm::vec3(0.0f, 0.0f, -1.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
			}
		}

		m_Camera.AddLocalOffset(locationOffsetLocal / 1000.0f);
		m_Camera.AddLocalRotation(rotationOffsetLocal / 1000.0f);
		m_Camera.AddWorldLocationOffset(locationOffsetWorld / 1000.0f);
	}

	void CameraController::OnEvent(Event& e) {

		VI_PROFILE_FUNCTION();


		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(VI_BINDEVENT(CameraController::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(VI_BINDEVENT(CameraController::OnKeyReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(VI_BINDEVENT(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(VI_BINDEVENT(CameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VI_BINDEVENT(CameraController::OnMouseButtonReleased));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(VI_BINDEVENT(CameraController::OnMouseMoved));

	}

	bool CameraController::OnKeyPressed(KeyPressedEvent& e) {
		VI_PROFILE_FUNCTION();

		if (true /*!Application::Get().GetImGuiLayer()->m_CapturingKeyboard*/) {

			if (e.GetKeyCode() == VI_KEY_LEFT_SHIFT) {
				m_BoostFactor = 3;
			}
		}
		return false;
	}

	bool CameraController::OnKeyReleased(KeyReleasedEvent& e) {
		VI_PROFILE_FUNCTION();

		if (e.GetKeyCode() == VI_KEY_LEFT_SHIFT) {
			m_BoostFactor = 1;
		}
		return false;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		if (true /*!Application::Get().GetImGuiLayer()->m_CapturingMouse*/) {

			if (!m_MouseRightButtonPressed) {
				m_Camera.AddLocalOffset(e.GetYOffset() * m_MouseScrollSensitivityMove);
			}
			else {
				//	float fovOffset = - e.GetYOffset() * 5.0f;			
				m_Camera.AddFOV(e.GetYOffset() * m_MouseScrollSensitivityFov * (-1.0f));
			}
		}
		return false;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
			
		if (true /*!Application::Get().GetImGuiLayer()->m_CapturingMouse*/) {

			if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE) {
				LastMiddleMouseXPos = Vi::Input::GetMousePosition().first;
				LastMiddleMouseYPos = Vi::Input::GetMousePosition().second;
				m_MouseMiddleButtonPressed = true;

				/*m_MouseRightButtonPressed = false;
				m_MouseLeftButtonPressed = false;*/
			}

			if (e.GetMouseButton() == VI_MOUSE_BUTTON_RIGHT) {
				LastRightMouseXPos = Vi::Input::GetMousePosition().first;
				LastRightMouseYPos = Vi::Input::GetMousePosition().second;
				m_MouseRightButtonPressed = true;
				m_FOVCache = m_Camera.GetFOV();

				/*m_MouseMiddleButtonPressed = false;
				m_MouseLeftButtonPressed = false;*/
			}

			if (e.GetMouseButton() == VI_MOUSE_BUTTON_LEFT) {
				LastLeftMouseXPos = Vi::Input::GetMousePosition().first;
				LastLeftMouseYPos = Vi::Input::GetMousePosition().second;
				m_MouseLeftButtonPressed = true;

				/*m_MouseRightButtonPressed = false;
				m_MouseMiddleButtonPressed = false;*/
			}

		}
		return false;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE) { m_MouseMiddleButtonPressed = false; }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_RIGHT) { m_MouseRightButtonPressed = false;		m_Camera.SetFOV(m_FOVCache); }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_LEFT) { m_MouseLeftButtonPressed = false; }
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e) {
	//	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	//	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (true /*!Application::Get().GetImGuiLayer()->m_CapturingMouse*/) {
			// Do logic
		}
		return false;
	}

}