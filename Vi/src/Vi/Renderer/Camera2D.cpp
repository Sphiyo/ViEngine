#include "Vipch.h"
#include "Camera2D.h"
 


namespace Vi {

	Camera2D::Camera2D(const VTransform& Transform, const Camera2DProperties& CamProperties) {
		VI_PROFILE_FUNCTION();
		
		m_ProjectionMatrix = glm::ortho(-CamProperties.AspectRatio, CamProperties.AspectRatio, -CamProperties.ZoomLevel, CamProperties.ZoomLevel, 
						     CamProperties.ZNear, CamProperties.ZFar);
		m_ViewMatrix = glm::mat4(1.0f);

		m_WorldForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
		m_WorldRightVector = glm::vec3(1.0f, 0.0f, 0.0f);
		m_WorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);


		m_WorldLocation = Transform.Location;
		m_RightVector = m_WorldRightVector;
		m_UpVector = m_WorldUpVector;

		SetWorldRotation(Transform.Rotation);
		 
		RecalculateViewMatrix();

		m_CamProperties.AspectRatio = CamProperties.AspectRatio;
		m_CamProperties.ZFar = CamProperties.ZFar;
		m_CamProperties.ZNear = CamProperties.ZNear;
		m_CamProperties.ZoomLevel = CamProperties.ZoomLevel;
	}

 
	
	void Camera2D::SetProjection(const Camera2DProperties& CamProperties) {

		VI_PROFILE_FUNCTION();
  
		m_ProjectionMatrix  = glm::ortho(-CamProperties.AspectRatio, CamProperties.AspectRatio, -CamProperties.ZoomLevel, CamProperties.ZoomLevel,
			                  CamProperties.ZNear, CamProperties.ZFar);
		 
		m_ViewProjectionMatrix  = m_ProjectionMatrix  * m_ViewMatrix;


		m_CamProperties.AspectRatio = CamProperties.AspectRatio;
		m_CamProperties.ZFar = CamProperties.ZFar;
		m_CamProperties.ZNear = CamProperties.ZNear;
		m_CamProperties.ZoomLevel = CamProperties.ZoomLevel;
	}


	void Camera2D::SetWorldLocation(float x, float y, float z)
	{
		m_WorldLocation = glm::vec3(x, y, z);

		RecalculateViewMatrix();
	}

	void Camera2D::AddLocalOffset(const glm::vec3& locationToAdd) {

		m_WorldLocation += m_ForwardVector * locationToAdd.x + m_RightVector * locationToAdd.y + m_UpVector * locationToAdd.z;
		RecalculateViewMatrix();
	}

	void Camera2D::AddWorldLocationOffset(float x, float y, float z) {

		m_WorldLocation += m_WorldForwardVector * x + m_WorldRightVector * y + m_WorldUpVector * z;
		RecalculateViewMatrix();
	}


	void Camera2D::SetWorldRotation(float pitch, float yaw, float roll) {

		m_WorldRotation = { pitch,yaw - 90.0f, roll }; 
		RecalculateViewMatrix();
	}

	void Camera2D::AddLocalRotation(const float& offsetYaw, const float& offsetPitch, const float& offsetRoll) {

		m_WorldRotation = { m_WorldRotation.x - offsetPitch, m_WorldRotation.y + offsetYaw , m_WorldRotation.z + offsetRoll };
		m_WorldRotation.x = glm::clamp<float>(m_WorldRotation.x, -89.0f, 89.0f);

		if (m_WorldRotation.y > 360.0f) { m_WorldRotation.y = 0.0f; }
		else if (m_WorldRotation.y < -360.0f) { m_WorldRotation.y = 0.0f; }
		if (m_WorldRotation.z > 360.0f) { m_WorldRotation.z = 0.0f; }
		else if (m_WorldRotation.z < -360.0f) { m_WorldRotation.z = 0.0f; }
		RecalculateViewMatrix();
	}

	const glm::vec3& Camera2D::GetForwardVector() const {
		return m_ForwardVector;
	}

	const glm::vec3& Camera2D::GetWorldForwardVector() const {
		return m_WorldForwardVector;
	}

	const glm::vec3& Camera2D::GetRightVector() const {
		return m_RightVector;
	}

	const glm::vec3& Camera2D::GetWorldRightVector() const {
		return m_WorldRightVector;
	}
	const glm::vec3& Camera2D::GetUpVector() const {
		return m_UpVector;
	}

	const glm::vec3& Camera2D::GetWorldUpVector() const {
		return m_WorldUpVector;
	}


	const glm::mat4& Camera2D::GetProjectionMatrix() const {
		return m_ProjectionMatrix;
	}

	const glm::mat4& Camera2D::GetViewProjectionMatrix() const {
		return m_ViewProjectionMatrix;
	}
	 

	void Camera2D::RecalculateViewMatrix() {
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
		m_ViewProjectionMatrix  = m_ProjectionMatrix  * m_ViewMatrix;
	}

}