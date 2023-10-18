#pragma once
#include "Vi/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <Vi/Math/VUtils.h>

enum class EMovementState : uint8_t {

	Static,
	Dynamic
};

struct Camera2DProperties { 

	Camera2DProperties(float aspectRatio = 1920.0f / 1080.0f, float zoomLevel = 1.0f,
		float zNear = -10.0f, float zFar = 10.0f,
		EMovementState movementState = EMovementState::Static)
		: AspectRatio(aspectRatio), ZoomLevel(zoomLevel),
		ZNear(zNear), ZFar(zFar), MovementState(movementState) {
	}


	float AspectRatio = 1920.0f / 1080.0f;
	float ZoomLevel = 1.0f;
	float ZNear = -10.0f;
	float ZFar = 10.0f;
	EMovementState MovementState = EMovementState::Static;
};

namespace Vi {

	class Camera2D
	{
	public:
		Camera2D(const VTransform& Transform = VTransform(), const Camera2DProperties& CamProperties = Camera2DProperties());

		~Camera2D() {}

		void SetProjection( const Camera2DProperties& CamProperties = Camera2DProperties());


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


		void AddLocalRotation(const glm::vec3& offsetRotation = glm::vec3(0.0f)) { AddLocalRotation(offsetRotation.x, offsetRotation.y, offsetRotation.z); }
		void AddLocalRotation(const float& offsetYaw = 0.0f, const float& offsetPitch = 0.0f, const float& offsetRoll = 0.0f);


		// World Directions
		const glm::vec3& GetWorldForwardVector() const;
		const glm::vec3& GetWorldRightVector() const;
		const glm::vec3& GetWorldUpVector() const;

		// Relative Directions
		const glm::vec3& GetForwardVector() const;
		const glm::vec3& GetRightVector() const;
		const glm::vec3& GetUpVector() const;

		 

		// Matrix s 
	
		const glm::mat4& GetProjectionMatrix() const;

		const glm::mat4& GetViewProjectionMatrix() const;



		const Camera2DProperties& GetCameraProperties() const { return m_CamProperties; }
		Camera2DProperties& GetCameraProperties() { return m_CamProperties; }

		void SetMovementState(EMovementState movementState) {
			m_CamProperties.MovementState = movementState;
		}

	private:
		
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;


		glm::vec3 m_WorldLocation;
		// m_WorldRotation(pitch, yaw, roll)
		glm::vec3 m_WorldRotation;


		glm::vec3 m_ForwardVector;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;


		glm::vec3 m_WorldForwardVector;
		glm::vec3 m_WorldRightVector;
		glm::vec3 m_WorldUpVector;
		 

		Camera2DProperties m_CamProperties;

	};
}