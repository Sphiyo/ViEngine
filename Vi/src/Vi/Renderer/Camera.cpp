#include "Vipch.h"
#include "Camera.h"



namespace Vi {

	Camera::Camera(const glm::vec3& worldLocation, const glm::vec3& rotation) {
		VI_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.7778f, 0.01f, 4000.f);
		m_ViewMatrix = glm::mat4(1.0f);

		m_WorldForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
		m_WorldRightVector = glm::vec3(1.0f, 0.0f, 0.0f);
		m_WorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

		
		m_WorldLocation = worldLocation;
		m_RightVector = m_WorldRightVector;
		m_UpVector = m_WorldUpVector;

		SetWorldRotation(rotation);
		

		m_FOV = 90.0f;
		CheckFOV();

		RecalculateViewMatrix();
	}

	void Camera::SetProjection(float fovy, float aspectratio, float nearPlane, float farPlane) {
		
		VI_PROFILE_FUNCTION();

		m_FOV = fovy;
		CheckFOV();
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), aspectratio, nearPlane, farPlane);
		m_ViewProjectionMatrix3D = m_ProjectionMatrix * m_ViewMatrix;

	}

	
	void Camera::SetWorldLocation(float x, float y, float z)
	{
		m_WorldLocation = glm::vec3(x, y, z);

		RecalculateViewMatrix();
	}

	void Camera::AddLocalOffset(const glm::vec3& locationToAdd) {

		m_WorldLocation += m_ForwardVector * locationToAdd.x + m_RightVector * locationToAdd.y + m_UpVector * locationToAdd.z;
		RecalculateViewMatrix();
	}

	void Camera::AddWorldLocationOffset(float x, float y, float z) {

		m_WorldLocation += m_WorldForwardVector * x + m_WorldRightVector * y + m_WorldUpVector * z;
		RecalculateViewMatrix();
	}


	void Camera::SetWorldRotation(float pitch, float yaw, float roll) {

		m_WorldRotation = { pitch,yaw - 90.0f, roll }; RecalculateViewMatrix();
	}

	void Camera::AddLocalRotation(const float& offsetYaw, const float& offsetPitch, const float& offsetRoll) {

		m_WorldRotation = { m_WorldRotation.x - offsetPitch, m_WorldRotation.y + offsetYaw , m_WorldRotation.z + offsetRoll };
		m_WorldRotation.x = glm::clamp<float>(m_WorldRotation.x, -89.0f, 89.0f);

		if (m_WorldRotation.y >  360.0f) { m_WorldRotation.y = 0.0f; }
		else if (m_WorldRotation.y < -360.0f) { m_WorldRotation.y = 0.0f; }
		if (m_WorldRotation.z >  360.0f) { m_WorldRotation.z = 0.0f; }
		else if (m_WorldRotation.z < -360.0f) { m_WorldRotation.z = 0.0f; }
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

	const float Camera::GetFOV() const {
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

	const glm::mat4& Camera::GetViewProjectionMatrix3D() const {
		return m_ViewProjectionMatrix3D;
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix() {
		VI_PROFILE_FUNCTION();

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
		m_ViewProjectionMatrix3D = m_ProjectionMatrix * m_ViewMatrix;
	}

}