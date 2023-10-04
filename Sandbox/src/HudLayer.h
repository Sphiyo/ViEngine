#pragma once
#include <Vi.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class HudLayer : public Vi::Layer {



public:

	HudLayer( );
	virtual ~HudLayer( ) = default;
	virtual void OnAttach( )override;
	virtual void OnDetach( )override;
	void OnUpdate( Vi::Timestep ts ) override;
	virtual void OnImGuiRender( ) override;
	void OnEvent( Vi::Event& e ) override;

	bool OnKeyPressed( Vi::KeyPressedEvent& e );


	int m_PlaneCount = 1;
	int m_CubeCount = 0;
private:
	glm::vec3 m_PlanePosition = glm::vec3( 0.0f );
	glm::vec3 m_PlaneRotation = glm::vec3( 0.0f );
	glm::vec3 m_PlaneScale = glm::vec3( 1.0f );


	static float deltaTime;

};