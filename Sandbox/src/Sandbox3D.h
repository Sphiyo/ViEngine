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
	virtual void OnImGuiRender() override;
	void OnEvent(Vi::Event& e) override;

private:
	Vi::CameraController m_CameraController;


	// temp 
	Vi::Ref<Vi::VertexArray> m_CubeVA;
	Vi::Ref<Vi::Shader> m_FlatColorShader;


	glm::vec4 m_CubeColor = glm::vec4(0.5f, 0.2f, 0.2f, 1.0f);
	glm::vec4 m_GroundColor = glm::vec4(0.2f, 0.2f, 0.5f, 1.0f);


	glm::vec3 m_Cube1Position = glm::vec3(0.0f);
	glm::vec3 m_Cube2Position = glm::vec3(0.0f);
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