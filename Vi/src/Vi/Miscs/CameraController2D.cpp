#include "Vipch.h"
#include "CameraController2D.h"

#include "Vi/Core/Input.h"
#include "Vi/Core/KeyCodes.h"
#include <Vi/Core/MouseButtonCodes.h>

#include "Vi/Core/Application.h"



Vi::CameraController2D* Vi::CameraController2D::s_CameraController2D = new Vi::CameraController2D();

namespace Vi {


	CameraController2D::CameraController2D(const glm::vec3& worldLocation, const glm::vec3& worldRotation)
		: m_Camera(VTransform(worldLocation, worldRotation)) {

	}


	void CameraController2D::OnUpdate(Timestep ts) {
		VI_PROFILE_FUNCTION();
		bool ShouldUpdate = false;
		//	VI_CORE_INFO("(CameraController2DCpp) TimeStep = {}", ts);
		glm::vec3 locationOffsetWorld(0.0f);
		glm::vec3 locationOffsetLocal(0.0f);
		glm::vec3 rotationOffsetLocal(0.0f);

		if (m_MouseMiddleButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMiddleMouseXPos;
			float YOffset = LastMiddleMouseYPos - Vi::Input::GetMouseY();
			locationOffsetWorld += glm::vec3(0.0f, 0.0f, YOffset * m_CameraTranslationSpeed * m_MouseYSensitivityMove);
			locationOffsetLocal += glm::vec3(0.0f, XOffset * m_MouseXSensitivityMove * m_CameraTranslationSpeed, 0.0f);

			ShouldUpdate = true;

			LastMiddleMouseXPos = Vi::Input::GetMouseX();
			LastMiddleMouseYPos = Vi::Input::GetMouseY();
		}

		if (m_MouseRightButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastRightMouseXPos;
			float YOffset = LastRightMouseYPos - Vi::Input::GetMouseY();

			rotationOffsetLocal += glm::vec3(XOffset * m_MouseXSensitivityLook * m_CameraRotationSpeed, -YOffset * m_MouseYSensitivityLook * m_CameraRotationSpeed, 0.0f);

			ShouldUpdate = true;

			LastRightMouseXPos = Vi::Input::GetMouseX();
			LastRightMouseYPos = Vi::Input::GetMouseY();
		}

		if (m_MouseLeftButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastLeftMouseXPos;
			float YOffset = LastLeftMouseYPos - Vi::Input::GetMouseY();

			locationOffsetWorld += glm::vec3(YOffset * m_CameraTranslationSpeed * m_MouseYSensitivityMove, 0.0f, 0.0f);
			rotationOffsetLocal += glm::vec3(XOffset * m_MouseXSensitivityLook * m_CameraRotationSpeed, 0.0f, 0.0f);

			ShouldUpdate = true;

			LastLeftMouseXPos = Vi::Input::GetMouseX();
			LastLeftMouseYPos = Vi::Input::GetMouseY();
		}

		bool IsLocallyControlled = true;
#if IMGUI == 1
		IsLocallyControlled = !Application::Get().GetImGuiLayer()->m_CapturingKeyboard;
#endif

		if (IsLocallyControlled) {

			if (Input::IsKeyPressed(VI_KEY_W) || Input::IsKeyPressed(VI_KEY_UP)) {
				locationOffsetLocal += glm::vec3(1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}

			if (Input::IsKeyPressed(VI_KEY_S) || Input::IsKeyPressed(VI_KEY_DOWN)) {
				locationOffsetLocal += glm::vec3(-1.0f, 0.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}

			if (Input::IsKeyPressed(VI_KEY_D) || Input::IsKeyPressed(VI_KEY_RIGHT)) {
				locationOffsetLocal += glm::vec3(0.0f, 1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}

			if (Input::IsKeyPressed(VI_KEY_A) || Input::IsKeyPressed(VI_KEY_LEFT)) {
				locationOffsetLocal += glm::vec3(0.0f, -1.0f, 0.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}

			if (Input::IsKeyPressed(VI_KEY_E)) {
				locationOffsetLocal += glm::vec3(0.0f, 0.0f, 1.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}
			if (Input::IsKeyPressed(VI_KEY_Q)) {
				locationOffsetLocal += glm::vec3(0.0f, 0.0f, -1.0f) * glm::vec3(m_CameraTranslationSpeed * m_BoostFactor * ts);
				ShouldUpdate = true;
			}
		}
		if (ShouldUpdate) {
			m_Camera.AddLocalOffset(locationOffsetLocal / 1000.0f);
			m_Camera.AddLocalRotation(rotationOffsetLocal / 1000.0f);
			m_Camera.AddWorldLocationOffset(locationOffsetWorld / 1000.0f);
		}
	}

	void CameraController2D::OnEvent(Event& e) {

		VI_PROFILE_FUNCTION();


		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(VI_BINDEVENT(CameraController2D::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(VI_BINDEVENT(CameraController2D::OnKeyReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(VI_BINDEVENT(CameraController2D::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(VI_BINDEVENT(CameraController2D::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VI_BINDEVENT(CameraController2D::OnMouseButtonReleased));
		dispatcher.Dispatch<WindowResizeEvent>(VI_BINDEVENT(CameraController2D::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(VI_BINDEVENT(CameraController2D::OnMouseMoved));

	}

	bool CameraController2D::OnKeyPressed(KeyPressedEvent& e) {
		VI_PROFILE_FUNCTION();
		bool IsLocallyControlled = true;
#if IMGUI == 1
		IsLocallyControlled = !Application::Get().GetImGuiLayer()->m_CapturingKeyboard;
#endif

		if (IsLocallyControlled) {

			if (e.GetKeyCode() == VI_KEY_LEFT_SHIFT) {
				m_BoostFactor = 12;
			}
		}
		return false;
	}

	bool CameraController2D::OnKeyReleased(KeyReleasedEvent& e) {
		VI_PROFILE_FUNCTION();

		if (e.GetKeyCode() == VI_KEY_LEFT_SHIFT) {
			m_BoostFactor = 1;
		}
		return false;
	}

	bool CameraController2D::OnMouseScrolled(MouseScrolledEvent& e) {
		bool IsLocallyControlled = true;
#if IMGUI == 1
		IsLocallyControlled = !Application::Get().GetImGuiLayer()->m_CapturingMouse;
#endif

		if (IsLocallyControlled) {

			if (!m_MouseRightButtonPressed) {
				m_Camera.AddLocalOffset(e.GetYOffset() * m_MouseScrollSensitivityMove);
			}
			else {
			}
		}
		return false;
	}

	bool CameraController2D::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		bool IsLocallyControlled = true;
#if IMGUI == 1
		IsLocallyControlled = !Application::Get().GetImGuiLayer()->m_CapturingMouse;
#endif

		if (IsLocallyControlled) {

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

	bool CameraController2D::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE) { m_MouseMiddleButtonPressed = false; }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_RIGHT) { m_MouseRightButtonPressed = false; }
		if (e.GetMouseButton() == VI_MOUSE_BUTTON_LEFT) { m_MouseLeftButtonPressed = false; }
		return false;
	}

	bool CameraController2D::OnWindowResized(WindowResizeEvent& e) {
		//	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		//	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool CameraController2D::OnMouseMoved(MouseMovedEvent& e)
	{
#if IMGUI == 1
		if (!Application::Get().GetImGuiLayer()->m_CapturingMouse) {
			// Do logic
		}
#endif
		return false;
	}

}