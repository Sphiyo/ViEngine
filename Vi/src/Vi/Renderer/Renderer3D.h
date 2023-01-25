#pragma once
#include "Camera.h"


namespace Vi {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();


		// Primitives
		static void DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color);
	};



}

