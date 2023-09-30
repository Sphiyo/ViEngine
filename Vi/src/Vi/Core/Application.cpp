#include "Vipch.h"
#include "Application.h"

#include "Vi/Renderer/Renderer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <GLFW/glfw3.h>

//#include "Vi/Debug/Timer.h"



template<typename Fn>
class Timer {

public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_Func(func), m_Stopped(false) {

		m_StartTimepoint = std::chrono::high_resolution_clock::now();

	}

	~Timer() {

		if (!m_Stopped)
			Stop();
	}

	void Stop() {

		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();

		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) / 1000.0f;
		m_Func({ m_Name, duration });
	}

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer vTimer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace Vi {

	Application* Application::s_Instance = nullptr;

	Application::Application( ) {

		VI_PROFILE_FUNCTION( );

		VI_CORE_ASSERT( !s_Instance, "(Application) Application Already Exists!" );
		s_Instance = this;
		m_Window = Window::Create( );
		m_Window->SetEventCallback( VI_BINDEVENT( Application::OnEvent ) );
		Renderer::Init( );

	//	Application* imGuiApp = new Application();

	#if IMGUI == 1


	#if IMGUITHREADED == 0
		m_ImGuiLayer = new ImGuiLayer( );
		PushOverlay( m_ImGuiLayer );
	#endif


	#if  IMGUITHREADED == 1
		m_ImGuiThread.AddFunction( [&] {
			m_ImGuiLayer = new ImGuiLayer( );
			PushOverlay( m_ImGuiLayer );
								   } );
		m_ImGuiThread.Run( );
	#endif

	#endif //  IMGUI == 1


	}

	Application::~Application( ) {
		VI_PROFILE_FUNCTION( );

		Renderer::Shutdown( );

	#if IMGUI == 1

	#if IMGUITHREADED == 1
		m_ImGuiThread.AddFunction( [&] {
			m_ImGuiLayer->OnDetach( );
								   } );
		m_ImGuiThread.Shutdown( );
	#endif

	#endif // IMGUI == 1
	}

	void Application::PushLayer( Layer* layer ) {
		VI_PROFILE_FUNCTION( );

		m_LayerStack.PushLayer( layer );
		layer->OnAttach( );
	}

	void Application::PushOverlay( Layer* overlay ) {
		VI_PROFILE_FUNCTION( );

		m_LayerStack.PushOverlay( overlay );
		overlay->OnAttach( );
	}

	void Application::PopOverlay( Layer* overlay ) {
		VI_PROFILE_FUNCTION( );

		m_LayerStack.PopOverlay( overlay );

	}


	void Application::OnEvent( Event& e ) {
		VI_PROFILE_FUNCTION( );

		EventDispatcher dispatcher( e );
		dispatcher.Dispatch<WindowCloseEvent>( VI_BINDEVENT( Application::OnWindowClosed ) );
		dispatcher.Dispatch<WindowResizeEvent>( VI_BINDEVENT( Application::OnWindowResize ) );
		dispatcher.Dispatch<KeyPressedEvent>( VI_BINDEVENT( Application::OnKeyPressed ) );							 // temp (move to editor)
		dispatcher.Dispatch<MouseButtonPressedEvent>( VI_BINDEVENT( Application::OnMouseButtonPressed ) );			 // temp (move to editor)
		dispatcher.Dispatch<MouseButtonReleasedEvent>( VI_BINDEVENT( Application::OnMouseButtonReleased ) );		 // temp (move to editor)
		dispatcher.Dispatch<MouseMovedEvent>( VI_BINDEVENT( Application::OnMouseMoved ) );							 // temp (move to editor)

		// LookForTheLastPossibleLayerThatCanRecieveEventAndIfItGetsHandledThenBreak
		for ( auto it = m_LayerStack.rbegin( ); it != m_LayerStack.rend( ); ++it ) {
			( *it )->OnEvent( e );
			if ( e.Handled )
				break;
		}
	}
	void Application::Run( ) {
		VI_PROFILE_FUNCTION( );

	//	PROFILE_SCOPE("Application::OnUpdate");

		while ( m_Running ) {

			VI_PROFILE_SCOPE( "RunLoop" );
			RendererAPI::ClearDrawCallCounts( );
			Timestep timestep = Time::GetTime( ) - m_LastFrameTime;
			m_LastFrameTime = Time::GetTime( );

			if ( !m_Minimized ) {

				static int count;
				VI_PROFILE_SCOPE( "LayerStack OnUpdate" );
				if( !IsRenderingPaused )
					m_Window->OnUpdate( );
				

				for ( Layer* layer : m_LayerStack ) {
					//if (layer == m_ImGuiLayer) continue;
				//	if(layer)		
					layer->OnUpdate( timestep );
				}

			}
		#if IMGUI == 1

		#if IMGUITHREADED == 1


			m_ImGuiThread.AddFunction( [&] {

				m_ImGuiLayer->Begin( );
				{


					for ( Layer* layer : m_LayerStack )																															   //
						layer->OnImGuiRender( );																																	   //

				}
				m_ImGuiLayer->End( );

									   } );

		#endif


		#if IMGUITHREADED == 0

			m_ImGuiLayer->Begin( );
			{
				VI_PROFILE_SCOPE( "LayerStack OnImGuiRender" );

				for ( Layer* layer : m_LayerStack )
					layer->OnImGuiRender( );

			}
			m_ImGuiLayer->End( );

		#endif

		#endif // IMGUI == 1
		};
	#if IMGUI == 1
	#if IMGUITHREADED == 0

		PopOverlay( m_ImGuiLayer );

	#endif

	#if IMGUITHREADED == 1

		m_ImGuiThread.AddFunction( [&] {
			PopOverlay( m_ImGuiLayer );

								   } );

	#endif

	#endif // IMGUI == 1

		


	}

//////////////////////////////////////////////////////////////////////////////////////////////////// revise /////////////////////////////////////////////////////////////////////////
																																												   //
	void Application::RunGUIAsync( )																																				   //
	{
	#if IMGUI == 1
	#if IMGUITHREADED == 1
			//
		std::thread ImGuiThread = std::thread( [&] {																																   //
																																												   //
			m_ImGuiLayer = new ImGuiLayer( );																																	   //
																																												   //
			m_LayerStack.PushOverlay( m_ImGuiLayer );																																   //
																																												   //
			m_ImGuiLayer->OnAttach( );																																			   //
																																												   //
			while ( !glfwWindowShouldClose( m_ImGuiLayer->m_ImGuiWindow ) ) {																										   //
																																												   //
				//	VI_CORE_WARN("ImGuiLayer Start loop");																														   //
				m_ImGuiLayer->Begin( );																																			   //
				{																																								   //
					VI_PROFILE_SCOPE( "LayerStack OnImGuiRender" );																												   //
																																												   //
					for ( Layer* layer : m_LayerStack )																															   //
						layer->OnImGuiRender( );																																	   //
					{																																							   //
						ImGui::Begin( "Profiler" );																																   //
																																												   //
						// remove delete to do to fix																															   //
						for ( auto& result : m_ProfileResults ) {																													   //
																																												   //
							char label[50];																																		   //
							strcpy_s( label, "%.3fms  " );																														   //
							strcat_s( label, result.Name );																														   //
							ImGui::Text( label, result.Time );																													   //
						}																																						   //
						m_ProfileResults.clear( );																																   //
						ImGui::End( );																																			   //
					}																																							   //
																																												   //
				}																																								   //
				//	VI_CORE_WARN("ImGuiLayer End loop");																														   //
				m_ImGuiLayer->End( );																																			   //
			}																																									   //
			m_ImGuiLayer->OnDetach( );																																			   //
																																												   //
											   } );																																									   //
		ImGuiThread.detach( );																																					   //
	#endif

	#endif // IMGUI == 1
	}																																											   //
																																												   //
	void Application::RunGUISync( )																																				   //
	{
	#if IMGUI == 1

	#if IMGUITHREADED == 0
		m_ImGuiLayer = new ImGuiLayer( );																																		   //
																																												   //
		m_LayerStack.PushOverlay( m_ImGuiLayer );																																	   //
																																												   //
		m_ImGuiLayer->OnAttach( );																																				   //
																																												   //
		while ( !glfwWindowShouldClose( m_ImGuiLayer->m_ImGuiWindow ) ) {																											   //
																																												   //
			//	VI_CORE_WARN("ImGuiLayer Start loop");																															   //
			m_ImGuiLayer->Begin( );																																				   //
			{																																									   //
				VI_PROFILE_SCOPE( "LayerStack OnImGuiRender" );																													   //
																																												   //
				for ( Layer* layer : m_LayerStack )																																   //
					layer->OnImGuiRender( );																																		   //
				{																																								   //
					ImGui::Begin( "Profiler" );																																	   //
																																												   //
					// remove delete to do to fix																																   //
					for ( auto& result : m_ProfileResults ) {																														   //
																																												   //
						char label[50];																																			   //
						strcpy_s( label, "%.3fms  " );																															   //
						strcat_s( label, result.Name );																															   //
						ImGui::Text( label, result.Time );																														   //
					}																																							   //
					m_ProfileResults.clear( );																																	   //
					ImGui::End( );																																				   //
				}																																								   //
																																												   //
			}																																									   //
			//	VI_CORE_WARN("ImGuiLayer End loop");																															   //
			m_ImGuiLayer->End( );																																				   //
		}																																										   //
		m_ImGuiLayer->OnDetach( );																																				   //
	#endif		

	#endif // IMGUI == 1																																							   //
	}																																											   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Application::OnWindowClosed( WindowCloseEvent& e ) {

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize( WindowResizeEvent& e ) {
		VI_PROFILE_FUNCTION( );

		if ( e.GetHeight( ) == 0 || e.GetWidth( ) == 0 ) {

			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize( e.GetWidth( ), e.GetHeight( ) );
		Application::Get( ).GetWindow( ).SetWindowAspectRatio( 16, 9 );

		return false;
	}

	bool Application::OnKeyPressed( KeyPressedEvent& e ) {

		if ( e.GetKeyCode( ) == VI_KEY_ESCAPE ) {

			CloseWindow( );
		};
		if ( e.GetKeyCode( ) == VI_KEY_F11 ) {

			m_Window->ToggleFullScreen( );
		};

		///// To Remove ////////////////////////
		if ( e.GetKeyCode( ) == VI_KEY_0 ) {

			m_Window->SetFullScreen( true, 0 );
		};

		if ( e.GetKeyCode( ) == VI_KEY_1 ) {

			m_Window->SetFullScreen( true, 1 );
		};

		if ( e.GetKeyCode( ) == VI_KEY_V ) {

			m_Window->ToggleVSync( );
		};
		////////////////////////////////////////


		return false;
	}

	bool Application::OnMouseButtonPressed( MouseButtonPressedEvent& e ) {

		bool IsLocallyControlled = true;

	#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
	#endif

		if ( IsLocallyControlled ) {
			m_Window->SetCursor( CursorState::DISABLED );
		}
		return false;
	}

	bool Application::OnMouseMoved( MouseMovedEvent& e ) {

		bool IsLocallyControlled = true;

	#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
	#endif

		if ( IsLocallyControlled ) {


		}
		return false;
	}

	bool Application::OnMouseButtonReleased( MouseButtonReleasedEvent& e ) {

		bool IsLocallyControlled = true;

	#if IMGUI == 1
		IsLocallyControlled = !m_ImGuiLayer->m_CapturingMouse;
	#endif

		if ( IsLocallyControlled ) {

			if ( Input::IsMouseButtonPressed( VI_MOUSE_BUTTON_LEFT ) || Input::IsMouseButtonPressed( VI_MOUSE_BUTTON_RIGHT ) || Input::IsMouseButtonPressed( VI_MOUSE_BUTTON_MIDDLE ) ) {
				//	m_Window->SetCursor(CursorState::DISABLED); 
				return false;
			}
		}
		m_Window->SetCursor( CursorState::NORMAL );
		return false;
	}
}