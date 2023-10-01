#pragma once
#include "Vi/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace Vi {

	class Camera
	{
	public:
		Camera(const glm::vec3& worldLocation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	
		~Camera() {}
		
		void SetProjection(float fovy = 90.0f, float aspectratio = 1.77778, float nearPlane = 0.01f, float farPlane = 4000.0f);


		const glm::vec3& GetWorldLocation() const { return m_WorldLocation; }
		// (pitch, yaw, roll)
		const glm::vec3& GetWorldRotation() const { return m_WorldRotation; }


		void SetWorldLocation(const glm::vec3& worldLocation) { SetWorldLocation(worldLocation.x, worldLocation.y, worldLocation.z); }
		void SetWorldLocation(float pitch, float yaw, float roll);


		void AddLocalOffset(float x = 0.0f, float y = 0.0f, float z = 0.0f) { AddLocalOffset({ x, y, z }); }
		void AddLocalOffset(const glm::vec3& locationToAdd);
		

		// Offset Location Ignoring Rotation
		void AddWorldLocationOffset(const glm::vec3& locationToAdd) { AddWorldLocationOffset(locationToAdd.x, locationToAdd.y, locationToAdd.z); }
		void AddWorldLocationOffset(float x, float y, float z);


		void SetWorldRotation(const glm::vec3& worldRotation) { SetWorldRotation(worldRotation.x, worldRotation.y, worldRotation.z); }
		void SetWorldRotation(float pitch, float yaw, float roll);


		void AddLocalRotation( const glm::vec3& offsetRotation = glm::vec3(0.0f)) { AddLocalRotation(offsetRotation.x, offsetRotation.y, offsetRotation.z); }
		void AddLocalRotation( const float& offsetYaw = 0.0f, const float& offsetPitch = 0.0f, const float& offsetRoll = 0.0f);
		

		// World Directions
		const glm::vec3& GetWorldForwardVector() const;
		const glm::vec3& GetWorldRightVector() const;
		const glm::vec3& GetWorldUpVector() const;

		// Relative Directions
		const glm::vec3& GetForwardVector() const;
		const glm::vec3& GetRightVector() const;
		const glm::vec3& GetUpVector() const;

		
		// FOV  
		const float GetFOV() const;
		void AddFOV(float fovToAdd);
		void SetFOV(float fov);
		void CheckFOV();

		// Matrix s 
		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix3D() const;
		
	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix3D;


		glm::vec3 m_WorldLocation;
		// m_WorldRotation(pitch, yaw, roll)
		glm::vec3 m_WorldRotation;
		

		glm::vec3 m_ForwardVector;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;


		glm::vec3 m_WorldForwardVector;
		glm::vec3 m_WorldRightVector;
		glm::vec3 m_WorldUpVector;

		float m_FOV = 90.0f;
		float m_MinFOV = 1.0f;
		float m_MaxFOV = 179.0f;

	};	
}	