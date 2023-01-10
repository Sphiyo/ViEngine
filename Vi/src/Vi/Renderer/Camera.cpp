#include "Vipch.h"
#include "Camera.h"



namespace Vi {

	Camera::Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& worldUpVector, float fovy) {
		
		m_ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.7778f, 0.01f, 4000.f);
		m_ViewMatrix = glm::mat4(1.0f);

		m_WorldForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
		m_WorldRightVector = glm::vec3(1.0f, 0.0f, 0.0f);
		m_WorldUpVector = worldUpVector;
		m_WorldLocation = position;
		m_RightVector = glm::vec3(0.0f);
		m_UpVector = worldUpVector;

		//	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		//	m_ViewMatrix = glm::mat4(1.0f);
		//	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		m_WorldRotation.x = 0.0f;
		m_WorldRotation.y = -90.0f;
		m_WorldRotation.z = 0.0f;

		m_FOV = fovy;
		CheckFOV();

		RecalculateViewMatrix();
	}

	void Camera::SetProjection(float fovy, float aspectratio, float nearPlane, float farPlane) {

		m_FOV = fovy;
		CheckFOV();
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), aspectratio, nearPlane, farPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	const glm::vec3& Camera::GetWorldLocation() const {
		return m_WorldLocation;
	}

	void Camera::SetWorldLocation(const glm::vec3& position) {

		m_WorldLocation = position; RecalculateViewMatrix();
	}

	void Camera::AddLocalOffset(const float& ts, const glm::vec3& locationToAdd) {

		m_WorldLocation += m_ForwardVector * locationToAdd.x * ts + m_RightVector * locationToAdd.y * ts + m_UpVector * locationToAdd.z * ts;
		RecalculateViewMatrix();
	}

	void Camera::AddForwardWorldOffset(const float& ts, const glm::vec3& locationToAdd) {

		m_WorldLocation += m_WorldForwardVector * locationToAdd.x * ts + m_WorldRightVector * locationToAdd.y * ts + m_WorldUpVector * locationToAdd.z * ts;
		RecalculateViewMatrix();
	}

	const glm::vec3& Camera::GetWorldRotation() const {

		return m_WorldRotation;
	}

	void Camera::SetWorldRotation(float pitch, float yaw, float roll) {

		m_WorldRotation = { pitch,yaw, roll }; RecalculateViewMatrix();
	}

	void Camera::AddLocalRotation(const float& ts, const float& offsetYaw, const float& offsetPitch) {

		m_WorldRotation = { m_WorldRotation.x - offsetPitch * ts, m_WorldRotation.y + offsetYaw * ts,0.0f };
		m_WorldRotation.x = glm::clamp<float>(m_WorldRotation.x, -89.0f, 89.0f);
		m_WorldRotation.y = glm::clamp<float>(m_WorldRotation.y, -360.0f, 360.0f);
		RecalculateViewMatrix();
	}

	const glm::vec3& Camera::GetForwardVector() const {
		return m_ForwardVector;
	}

	const glm::vec3& Camera::GetWorldForwardVector() const {
		return m_WorldForwardVector;
	}

	const glm::vec3& Camera::GetRightVector() const {
		return m_RightVector;
	}

	const glm::vec3& Camera::GetWorldRightVector() const {
		return m_WorldRightVector;
	}
	const glm::vec3& Camera::GetUpVector() const {
		return m_UpVector;
	}

	const glm::vec3& Camera::GetWorldUpVector() const {
		return m_WorldUpVector;
	}

	float Camera::GetFOV() const {
		return m_FOV;
	}

	void Camera::AddFOV(float fovToAdd) {

		m_FOV += fovToAdd;
		SetProjection(m_FOV);
	}

	void Camera::SetFOV(float fov) {

		m_FOV = fov;
		SetProjection(m_FOV);
	}

	void Camera::CheckFOV() {

		m_FOV = glm::clamp<float>(m_FOV, m_MinFOV, m_MaxFOV);
	}

	const glm::mat4& Camera::GetProjectionMatrix() const {
		return m_ProjectionMatrix;
	}

	const glm::mat4& Camera::GetViewProjectionMatrix() const {
		return m_ViewProjectionMatrix;
	}

	void Camera::RecalculateViewMatrix() {

		// local vecs
		m_ForwardVector.x = cos(glm::radians(m_WorldRotation.y)) * cos(glm::radians(m_WorldRotation.x));
		m_ForwardVector.y = sin(glm::radians(m_WorldRotation.x));
		m_ForwardVector.z = sin(glm::radians(m_WorldRotation.y)) * cos(glm::radians(m_WorldRotation.x));

		m_ForwardVector = glm::normalize(m_ForwardVector);
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, m_WorldUpVector));
		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));

		// world vecs
		m_WorldForwardVector.x = cos(glm::radians(m_WorldRotation.y)) * cos(glm::radians(0.0f));
		m_WorldForwardVector.y = sin(glm::radians(0.0f));
		m_WorldForwardVector.z = sin(glm::radians(m_WorldRotation.y)) * cos(glm::radians(0.0f));

		m_WorldForwardVector = glm::normalize(m_WorldForwardVector);
		m_WorldRightVector = glm::normalize(glm::cross(m_WorldForwardVector, m_WorldUpVector));
		m_WorldUpVector = glm::normalize(glm::cross(m_WorldRightVector, m_WorldForwardVector));

		m_ViewMatrix = glm::lookAt(m_WorldLocation, m_WorldLocation + m_ForwardVector, m_UpVector);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}