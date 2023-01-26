#pragma once
#include "Camera.h"
#include "Texture.h"


namespace Vi {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();


		// Primitives 
		static void DrawCube (const glm::vec3& position = { 0.0f,0.0f,0.0f }, const glm::vec3& rotation = { 0.0f,0.0f,0.0f }, const glm::vec3& scale = { 1.0f,1.0f,1.0f }, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
		static void DrawCube (const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });

		static void DrawPlane(const glm::vec3& position = { 0.0f,0.0f,0.0f }, const glm::vec3& rotation = { 0.0f,0.0f,0.0f }, const glm::vec3& scale = { 1.0f,1.0f,1.0f }, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
		static void DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
	};



}

