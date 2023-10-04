#pragma once
#include <Vi.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

class Sandbox3D : public Vi::Layer {
	

public:

	Sandbox3D();
	virtual ~Sandbox3D() = default;
	virtual void OnAttach()override;
	virtual void OnDetach()override;
	void OnUpdate(Vi::Timestep ts) override;
	void OnFixedUpdate(Vi::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vi::Event& e) override;

	bool OnKeyPressed(Vi::KeyPressedEvent& e);

private:
	


	// temp 
	Vi::Ref<Vi::VertexArray> m_CubeVA;
	Vi::Ref<Vi::Shader> m_FlatColorShader;

	Vi::Ref<Vi::Texture2D> m_Texture, m_TransparentTexture, m_CheckerTexture;

	glm::vec4 m_CubeColor = glm::vec4(0.2f, 0.3f, 0.1f, 1.0f);
	glm::vec4 m_GroundColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	glm::ivec2 m_GridCount = { 3, 1 };
	glm::vec2 m_GridOffset = { 3.5f,2.0f };

	
	uint32_t m_CubeCountPx = 1;
	uint32_t m_CubeCount = 1;

	glm::vec3 m_Cube1Position = glm::vec3(0.0f);
	glm::vec3 m_GroundLocation = glm::vec3(0.0f);
	glm::vec3 m_CubeGridLocation = glm::vec3(0.0f);
	glm::vec3 m_Cube4Position = glm::vec3(0.0f);

	float LastMouseXPos = 0.f;				// temp
	float LastMouseYPos = 0.f;				// temp
	bool m_MouseRightButtonPressed = false;  // temp
	bool m_MouseLeftButtonPressed = false;  // temp
	bool m_MouseMiddleButtonPressed = false;  // temp
	/*glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
	float m_SquareRotation = 0.0f;
	float m_SquareRotateSpeed = 180.0f;*/

	static float deltaTime;				// temp
	bool vSync = true;                          // temp
	bool fullScreen = false;                          // temp
};