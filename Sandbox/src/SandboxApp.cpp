#include <Vi.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
//		m_Camera(90.0f, 800.0f / 600.0f, 0.1f, 4000.0f)
class ExampleLayer : public Vi::Layer {

public:
	ExampleLayer()
		: Layer("Example") , m_CameraController(1600.0f/900.0f) {

	
		// VertexArray 
		m_VertexArray.reset(Vi::VertexArray::Create());

		// VertexBuffer 
		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,    0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,    1.0f, 0.0f
		};
		Vi::Ref<Vi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Vi::VertexBuffer::Create(vertices, sizeof(vertices)));
		Vi::BufferLayout layout = {
			{ Vi::ShaderDataType::Float3, "a_Position" },
			{ Vi::ShaderDataType::Float2, "a_TexCoord" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer 
		Vi::Ref<Vi::IndexBuffer> indexBuffer;
		uint32_t indices[6] = { 0,1,2,2,3,0 };
		indexBuffer.reset(Vi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		
		// Square VertexArray 
		m_SquareVA.reset(Vi::VertexArray::Create());

		// Square VertexBuffer 
		float squareVertices[5 * 4 * 2] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
					  
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f					  
		};
		Vi::Ref<Vi::VertexBuffer> squareVB;
		squareVB.reset(Vi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Vi::ShaderDataType::Float3, "a_Position" },
			{ Vi::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		
		// Square IndexBuffer 
		Vi::Ref<Vi::IndexBuffer> squareIB;
		uint32_t squareIndices[6*6] = {
			0,1,2,2,3,0,
			0,4,7,7,3,0,
			4,5,6,6,7,4,
			5,1,2,2,6,5,
			3,2,6,6,7,3,
			0,1,5,5,4,0
		};
		squareIB.reset(Vi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// ShaderSource Vertex 
		std::string vertexSrc = R"(
			#version 330 core
	
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
	
			out vec3 v_Position;
			out vec4 v_Color;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
			}
		)";
		// ShaderSource Fragment 
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
	
		// Create And Compile Shader 
		m_Shader = Vi::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		// flat ShaderSource Vertex 
		std::string flatColorVertexSrc = R"(
			#version 330 core
	
			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
	
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
			}
		)";
		// flat ShaderSource Fragment 	
		std::string flatColorFragmentSrc = R"(
			#version 330 core
	
			layout(location = 0) out vec4 color;
	
			in vec3 v_Position;
	
			uniform vec4 u_Color;
	
			void main()
			{
				color = u_Color;
				
			}
		)";

		// Create And Compile flat Shader 
		m_FlatColorShader = Vi::Shader::Create("FlatColorShader",flatColorVertexSrc, flatColorFragmentSrc);

		// Create And Compile texture Shader 
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		//Construct And Buffer Texture To Memory
		m_Texture = Vi::Texture2D::Create("assets/textures/Cat03.jpg", 0);
		m_TransparentTexture = Vi::Texture2D::Create("assets/textures/RGBA.png", 0);

		// bind texture Shader And Set Uniform
		std::dynamic_pointer_cast<Vi::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Vi::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		Vi::Application::Get().GetWindow().SetResolution(2560, 1440);
	}

	virtual void OnUpdate(Vi::Timestep ts) override	{

		ExampleLayer::deltaTime = ts;										//temp

		// Set Background Color And Clear It 
		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();
			
		Vi::Renderer::BeginScene3D(m_CameraController.GetCamera3D());
		m_CameraController.OnUpdate(ts);

		//Vi::MaterialRef material = new Vi::Material(m_FlatColorShader);        				// todo 
		//Vi::MaterialInstanceRef mi = new Vi::MaterialInstance(material);						// todo 
		//
		//mi->SetValue("u_Color", firstColor);													// todo 
		//mi->SetTexture("u_Color", texture);													// todo 
		//mesh->SetMaterial(material);															// todo 

		//Bind FlatColor Shader And Set Uniform Color 
		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_ShaderColor);
				
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 25; y++) {
			for (int x = 0; x < 10; x++) {
				
				glm::vec3 pos(x * 0.211f, 0.0f, y * 0.211f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				m_Texture->Bind();
				Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
	 
	//	Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);									// todo 
	//	Vi::Renderer::Submit(mi, m_SquareVA, transform);												// todo  

		auto textureShader = m_ShaderLibrary.Get("Texture");
	
		// Draw Cube With Texture 
		m_Texture->Bind();
		Vi::Renderer::Submit(textureShader, m_SquareVA);
		
		// Draw Cube With Alpha Blend Texture 
		m_TransparentTexture->Bind();
		Vi::Renderer::Submit(textureShader, m_SquareVA);
	
		
		Vi::Renderer::EndScene();
		fullScreen = Vi::Application::Get().GetWindow().IsFullScreen();		// temp 
	}

	virtual void OnImGuiRender() override {

		auto& window = Vi::Application::Get().GetWindow();
		ImGui::Begin("Settings");														 // temp
		ImGui::ColorEdit4("ColorShader", (float*)&m_ShaderColor);
		ImGui::End();																   // temp
		
		


		ImGui::Begin("Stats");														   // temp 
		ImGui::Text("Fps  %.0fFps", 1/ ExampleLayer::deltaTime);
		ImGui::Text("ts  %.3fms" , ExampleLayer::deltaTime);
		ImGui::End();

		ImGui::Begin("GraphicsSetting");		
		ImGui::Checkbox("Vsync", &vSync);
		window.SetVSync(vSync);
		bool fullscreen = window.IsFullScreen();
		ImGui::Checkbox("FullScreen", &fullScreen);
		if (fullScreen != fullscreen) {
			if (fullScreen) {
				window.SetFullScreen(true);
			}
			else {
				window.SetFullScreen(false);
			}
		}
		const char* items[] = { "800x600","1600x900","1920x1080", "2560x1440" };
		static const char* current_item = NULL;
		int temporary = -1;
		if (ImGui::BeginCombo("##Resolution", current_item))
		{
			
			
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
				{
					current_item = items[n];
					temporary = n;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					
					
				}
			}

			ImGui::EndCombo();
			switch (temporary)
			{
			case 0:
				window.SetResolution(800, 600);
				break;
			case 1:
				window.SetResolution(1600, 900);
				break;
			case 2:
				window.SetResolution(1920, 1080);
				break;
			case 3:
				window.SetResolution(2560, 1440);
				break;
			default:
				break;
			}
		}

		
		ImGui::End();
	}

	void OnEvent(Vi::Event& e) override {

		m_CameraController.OnEvent(e);

		if (e.GetEventType() == Vi::EventType::WindowResize)
		{
			auto resizeEvent = (Vi::WindowResizeEvent&)e;			
		}
	}
	

private:

	Vi::ShaderLibrary m_ShaderLibrary;
	Vi::Ref<Vi::Shader> m_Shader;
	Vi::Ref<Vi::VertexArray> m_VertexArray;


	Vi::Ref<Vi::Shader> m_FlatColorShader;
	Vi::Ref<Vi::VertexArray> m_SquareVA;

	Vi::Ref<Vi::Texture2D> m_Texture, m_TransparentTexture;

	Vi::CameraController m_CameraController;

	glm::vec3 m_CameraPosition = glm::vec3(0.0f);


	glm::vec4 m_ShaderColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
//	glm::vec4 secondColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

	float LastMouseXPos;				// temp
	float LastMouseYPos;				// temp
	bool m_MouseRightButtonPressed = false;  // temp
	bool m_MouseLeftButtonPressed = false;  // temp
	bool m_MouseMiddleButtonPressed = false;  // temp
	/*glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
	float m_SquareRotation = 0.0f;
	float m_SquareRotateSpeed = 180.0f;*/

	static float deltaTime;				// temp
	bool vSync;                          // temp
	bool fullScreen;                          // temp
};
float ExampleLayer::deltaTime = 0.016f;

class Sandbox : public Vi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
		
	}
};


Vi::Application* Vi::CreateApplication() {

	return new Sandbox();
}
