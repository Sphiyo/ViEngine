#include "Vipch.h"
#include "Application.h"

#include "Vi/Log.h"

#include "Vi/Renderer/Renderer.h"

#include "Input.h"
#include "KeyCodes.h" // temp


namespace Vi {

//#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1 )

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{

		VI_CORE_ASSERT(!s_Instance, "Application Already Exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(VI_BINDEVENT(Application::OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());
		

	/*	glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);*/

	
		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create( vertices, sizeof(vertices)));		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		
		
		std::shared_ptr<IndexBuffer> indexBuffer;
		uint32_t indices[3] = { 0,1,2 };	
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);





		/// <summary>
		/// ////
		/// </summary>

		m_SquareVA.reset(VertexArray::Create());
	
		float squareVertices[4 * 3] = {
		-0.75f, -0.75f, 0.0f,
		-0.75f,  0.75f, 0.0f,
		 0.75f,  0.75f, 0.0f,
		 0.75f, -0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;		
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	


		squareVB->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);


		std::shared_ptr<IndexBuffer> squareIB;
		uint32_t squareIndices[6] = { 0,1,2, 2, 3, 0 };
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		/// <summary>
		/// /////
		/// </summary>

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5 ,1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		/// <summary>
		/// ////
		/// </summary>
		std::string blueVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.0 , 0.0 , 0.8 ,1.0);
				
			}
		)";
		
		blueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));
		

		
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VI_BINDEVENT(Application::OnWindowClosed));

		// or my implementation
		//if (e.GetEventType() == EventType::WindowClose) { OnWindowClosed(*(WindowCloseEvent*)&e); }
		//
		//
		/*VI_CORE_TRACE("{0}",e);*/

		// LookForTheLastPossibleLayerThatCanRecieveEventAndIfItGetsHandledThenBreak
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run() {

		
		while (m_Running) {

			
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		
			if (Input::IsKeyPressed(VI_KEY_W)) { position.y -= 0.01f; }           // temp 
			if (Input::IsKeyPressed(VI_KEY_S)) { position.y += 0.01f; }           // temp 
			if (Input::IsKeyPressed(VI_KEY_A)) { position.x += 0.01f; }           // temp 
			if (Input::IsKeyPressed(VI_KEY_D)) { position.x -= 0.01f; }           // temp 
			if (Input::IsKeyPressed(VI_KEY_Q)) { rotation -= 0.1f; }              // temp 
			if (Input::IsKeyPressed(VI_KEY_E)) { rotation += 0.1f; }              // temp 
																		          
			m_Camera.SetPosition(position);                                       // temp
			m_Camera.SetRotation(rotation);                                       // temp
			Renderer::BeginScene(m_Camera);

			
			Renderer::Submit(blueShader, m_SquareVA);

			
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
	
		};
	}


	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}