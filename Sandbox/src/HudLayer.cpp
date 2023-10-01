#include "HudLayer.h"

float HudLayer::deltaTime = 0.016f;


HudLayer::HudLayer( ) :
	Layer( "HudLayer" )  {

	VI_PROFILE_FUNCTION( );
	
}

void HudLayer::OnAttach( ) {
}

void HudLayer::OnDetach( ) {
}

void HudLayer::OnUpdate( Vi::Timestep ts ) {
	deltaTime = ts.GetMilliSeconds( );
	VI_PROFILE_FUNCTION( );
	VI_CORE_WARN( "HudLayer.cpp: OnUpdate(): Timestep is {} : FPS = {}", deltaTime, 1000 / deltaTime );



	{
		VI_PROFILE_SCOPE( "CameraController::OnUpdate" );
		Vi::CameraController::Get().OnUpdate( ts );

	}

	//{
	//	VI_PROFILE_SCOPE( "Renderer Prep" );
	//	Vi::RenderCommand::SetClearColor( { 0.1f, 0.1f, 0.1f, 1 } );
	//	Vi::RenderCommand::Clear( );
	//}

	
	//Vi::Renderer3D::BeginScene( s_CameraController.GetCamera( ) );


	{
		VI_PROFILE_SCOPE( "Renderer Draw" );
	
		//Vi::Renderer2D::DrawCanvas( s_CameraController.GetCamera( ), { 0.0f ,0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
		//Vi::Renderer2D::DrawCanvas( s_CameraController.GetCamera( ), { 0.0f ,0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );

		//Vi::Renderer3D::DrawCube( { 0.0f , 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,3.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
		float dist = 3000;
		for(int i =0;i< m_PlaneCount;i++ )
			Vi::Renderer3D::DrawPlane( { 0.0f  ,i * 30 ,i * 3 }, { 0.0f, 0.0f, 0.0f }, { dist,dist,dist }, { 0.2f,0.2f,0.2f, 1.0f } );
	
	//	Vi::Renderer3D::DrawPlane( { 0.0f  , dist /2  ,   0.0f   }, {  0.0f, 0.0f, 180.0f }, { dist,dist,dist }, { 0.2f,0.5f,0.8f, 1.0f } );
	//	Vi::Renderer3D::DrawPlane( { 0.0f  ,   0.0f   ,  dist /2 }, { 90.0f, 0.0f, 0.0f }, { dist,dist,dist }, { 0.2f,0.5f,0.8f, 1.0f } );
	//	Vi::Renderer3D::DrawPlane( { 0.0f  ,   0.0f   , -dist /2 }, { 90.0f, 0.0f, 0.0f }, { dist,dist,dist }, { 0.2f,0.5f,0.8f, 1.0f } );
	//	Vi::Renderer3D::DrawPlane( { 0.0f  ,-dist / 2 ,   0.0f   }, { 0.0f ,90.0f, 0.0f }, { dist,dist,dist }, { 0.2f,0.5f,0.8f, 1.0f } );
		for ( int i = 0; i < m_CubeCount; i++ )
		Vi::Renderer3D::DrawCube( { 0.0f  ,i*8  ,0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 0.15f,0.1f,0.1f, 1.0f } );
		
		
		Vi::Renderer3D::DrawCubeInversed( { 0.0f  ,0.0f  ,0.0f }, { 0.0f, 0.0f, 0.0f }, { dist,dist,dist }, { 0.2f,0.5f,0.8f, 1.0f } );
	}
	//Vi::Renderer3D::EndScene( );

//	Vi::Renderer3D::BeginScene( s_CameraController.GetCamera( ) );
//	
//	
//	{
//		VI_PROFILE_SCOPE( "Renderer Draw" );
//	
//		//Vi::Renderer2D::DrawCanvas( s_CameraController.GetCamera( ), { 0.0f ,0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//		//Vi::Renderer2D::DrawCanvas( s_CameraController.GetCamera( ), { 0.0f ,0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//		//Vi::Renderer2D::DrawCanvas( s_CameraController.GetCamera( ), { 0.0f ,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//		Vi::Renderer3D::DrawCube( { 0.0f ,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//	
//		//Vi::Renderer2D::DrawPlane( { 0.0f ,0.0f, 0.0f }, { 0.0f, 0.0f, 90.0f }, { 100.0f,0.0f,100.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//		//Vi::Renderer3D::DrawCube( { 0.0f , 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,3.0f,1.0f }, { 1.0f,0.0f,1.0f, 1.0f } );
//	
//	}
//	Vi::Renderer3D::EndScene( );


}

void HudLayer::OnImGuiRender( ) {
	ImGui::Begin( "Settings" );														 // temp
	


	float tempPosition[3];

	{
		// m_Cube1Position
		tempPosition[0] = m_PlanePosition.x;
		tempPosition[1] = m_PlanePosition.y;
		tempPosition[2] = m_PlanePosition.z;

		ImGui::DragFloat3( "Cube1Position", tempPosition, 0.01f );
		m_PlanePosition.x = tempPosition[0];
		m_PlanePosition.y = tempPosition[1];
		m_PlanePosition.z = tempPosition[2];


	}	
	ImGui::End( );

	ImGui::Begin( "Stats" );														   // temp 
	ImGui::Text( "Fps  %.0fFps", 1000 / deltaTime );
	ImGui::Text( "ts  %.3fms", deltaTime );
	ImGui::End( );
}

void HudLayer::OnEvent( Vi::Event& e ) {
	Vi::CameraController::Get( ).OnEvent( e );
	Vi::EventDispatcher dispacher( e );
	dispacher.Dispatch<Vi::KeyPressedEvent>( VI_BINDEVENT( HudLayer::OnKeyPressed ) );
}

bool HudLayer::OnKeyPressed( Vi::KeyPressedEvent& e ) {

	if(e.GetKeyCode( ) == Vi::KeyCode::Space)
		m_PlaneCount++;
	
	if(e.GetKeyCode( ) == Vi::KeyCode::Enter)
		m_CubeCount++;

	return false;
}
