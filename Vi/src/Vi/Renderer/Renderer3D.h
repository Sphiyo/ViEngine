#pragma once
#include "Camera3D.h"
#include "Texture.h"


#include <glm/gtc/quaternion.hpp>

namespace Vi {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera3D& camera);
		static void EndScene();
		static void Flush();

		// Primitives 
		static void DrawCube(const glm::vec3& position = glm::vec3(0), const glm::vec3& rotation = glm::vec3(0), const glm::vec3& scale = glm::vec3(1), const glm::vec4& color = glm::vec4(1), float tilingFactor = 1.0f);
		static void DrawCube(const glm::vec3& position = glm::vec3(0), const glm::quat& rotation = glm::quat(), const glm::vec3& scale = glm::vec3(1), const glm::vec4& color = glm::vec4(1), float tilingFactor = 1.0f);
		static void DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1), float tilingFactor = 1.0f);

		static void DrawPlane(const glm::vec3& position = glm::vec3(0), const glm::quat& rotation = glm::quat(), const glm::vec3& scale = glm::vec3(1), const glm::vec4& color = glm::vec4(1), float tilingFactor = 1.0f);
		static void DrawPlane(const glm::vec3& position = glm::vec3(0), const glm::vec3& rotation = glm::vec3(0), const glm::vec3& scale = glm::vec3(1), const glm::vec4& color = glm::vec4(1), float tilingFactor = 1.0f);
		static void DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1), float tilingFactor = 1.0f);
	};



}

