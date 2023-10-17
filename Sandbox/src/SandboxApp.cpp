#include <Vi.h>
#include <Vi/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox3D.h"
#if 0
class ExampleLayer : public Vi::Layer {

public:
	ExampleLayer()
		: Layer("Example") , m_CameraControllerEX() {



	
		// VertexArray 
		m_VertexArray = Vi::VertexArray::Create();

		// VertexBuffer 
		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,    0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,    1.0f, 0.0f
		};
		Vi::Ref<Vi::VertexBuffer> vertexBuffer = Vi::VertexBuffer::Create(vertices, sizeof(vertices));
		Vi::BufferLayout layout = {
			{ Vi::ShaderDataType::Float3, "a_Position" },
			{ Vi::ShaderDataType::Float2, "a_TexCoord" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer 
		
		uint32_t indices[6] = { 0,1,2,2,3,0 };
		Vi::Ref<Vi::IndexBuffer> indexBuffer = Vi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		
		// Square VertexArray 
		m_CubeVA = Vi::VertexArray::Create();
		
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
		Vi::Ref<Vi::VertexBuffer> squareVB = Vi::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Vi::ShaderDataType::Float3, "a_Position" },
			{ Vi::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_CubeVA->AddVertexBuffer(squareVB);
		
		// Square IndexBuffer 
		
		uint32_t squareIndices[6*6] = {
			0,1,2,2,3,0,
			0,4,7,7,3,0,
			4,5,6,6,7,4,
			5,1,2,2,6,5,
			3,2,6,6,7,3,
			0,1,5,5,4,0
		};
		Vi::Ref<Vi::IndexBuffer> squareIB = Vi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_CubeVA->SetIndexBuffer(squareIB);

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
		m_TextureEX = Vi::Texture2D::Create("assets/textures/Cat03.jpg", 0);
		m_TransparentTextureEX = Vi::Texture2D::Create("assets/textures/RGBA.png", 0);

		// bind texture Shader And Set Uniform
		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	//	Vi::Application::Get().GetWindow().SetResolution(1600, 900);

		
	}

	virtual void OnUpdate(Vi::Timestep ts) override	{
		
	

		
		

		ExampleLayer::deltaTime = ts;										//temp
		m_CameraControllerEX.OnUpdate(ts);

		// Set Background Color And Clear It 
		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();
			
		Vi::Renderer::BeginScene(m_CameraControllerEX.GetCamera());

		//Vi::MaterialRef material = new Vi::Material(m_FlatColorShader);        				// todo 
		//Vi::MaterialInstanceRef mi = new Vi::MaterialInstance(material);						// todo 
		//
		//mi->SetValue("u_Color", firstColor);													// todo 
		//mi->SetTexture("u_Color", texture);													// todo 
		//mesh->SetMaterial(material);															// todo 

		//Bind FlatColor Shader And Set Uniform Color 
		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat4("u_Color", m_CubeColorEX);


		const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 0.5f, 1.5f));
		
		for (int x = 0; x < 5; x++) {
				
				const glm::vec3 pos( 1.5f, 1.0f /2.0f + x,- x * 1.5f);
				const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				m_TextureEX->Bind();
				Vi::Renderer::Submit(m_FlatColorShader, m_CubeVA, transform);
			
		}
	 
	//	Vi::Renderer::Submit(m_FlatColorShader, m_CubeVA, transform);									// todo 
	//	Vi::Renderer::Submit(mi, m_CubeVA, transform);												// todo  
		auto textureShader = m_ShaderLibrary.Get("Texture");
	
		// Draw Cube With Texture 
		m_TextureEX->Bind();
		Vi::Renderer::Submit(textureShader, m_CubeVA);
		
		// Draw Cube With Alpha Blend Texture 
		m_TransparentTextureEX->Bind();
		Vi::Renderer::Submit(textureShader, m_CubeVA);
	
		{
			const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, 1000.0f));
			
			const glm::vec3 pos(15.f, 0.0f, 15.f);

			const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

			m_FlatColorShader->Bind();
			m_FlatColorShader->SetFloat4("u_Color", m_GroundColorEX);
			
		//	std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->Bind();
		//	std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_GroundColorEX);
			Vi::Renderer::Submit(m_FlatColorShader, m_CubeVA, transform);
		}
		
		
		Vi::Renderer::EndScene();
		fullScreen = Vi::Application::Get().GetWindow().IsFullScreen();		// temp 

		
	}

	virtual void OnImGuiRender() override {



		
		auto& window = Vi::Application::Get().GetWindow();
		ImGui::Begin("SettingsEX");														 // temp
		ImGui::ColorEdit4("ColorShaderEX", (float*)&m_CubeColorEX);
		ImGui::ColorEdit4("GroundShaderEX", (float*)&m_GroundColorEX);
		ImGui::End();																   // temp
		
		


		ImGui::Begin("StatsEX");														   // temp 
		ImGui::Text("Fps  %.0fFps", 1/ ExampleLayer::deltaTime);
		ImGui::Text("ts  %.3fms" , ExampleLayer::deltaTime);
		ImGui::End();

		ImGui::Begin("GraphicsSettingEX");		
		ImGui::Checkbox("VsyncEX", &vSync);
		window.SetVSync(vSync);
		bool fullscreen = window.IsFullScreen();
		ImGui::Checkbox("FullScreenEX", &fullScreen);
		if (fullScreen != fullscreen) {
			if (fullScreen) {
				window.SetFullScreen(true);
			}
			else {
				window.SetFullScreen(false);
			}
		}
		const char* items[] = { 
			"800x600","1024x768","1128x634","1152x864","1280x600",
			"1280x720","1280x768","1280x960","1280x1024","1360x768",
			"1366x768","1440x900","1600x900","1600x1200","1680x1050",
			"1760x990","1920x1080","1920x1200", "2560x1440"};
		static const char* current_item = NULL;
		int temporary = -1;
		if (ImGui::BeginCombo("##ResolutionEX", current_item))
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
				window.SetResolution(1024, 768);
				break;
			case 2:
				window.SetResolution(1128, 634);
				break;
			case 3:
				window.SetResolution(1152, 900);
				break;
			case 4:
				window.SetResolution(1280, 600);
				break;
			case 5:
				window.SetResolution(1280, 720);
				break;
			case 6:
				window.SetResolution(1280, 786);
				break;
			case 7:
				window.SetResolution(1280, 960);
				break;
			case 8:
				window.SetResolution(1280, 1024);
				break;
			case 9:
				window.SetResolution(1360, 768);
				break;
			case 10:
				window.SetResolution(1366, 768);
				break;
			case 11:
				window.SetResolution(1440, 900);
				break;
			case 12:
				window.SetResolution(1600, 900);
				break;
			case 13:
				window.SetResolution(1600, 1200);
				break;
			case 14:
				window.SetResolution(1680, 1050);
				break;
			case 15:
				window.SetResolution(1760, 990);
				break;
			case 16:
				window.SetResolution(1920, 1080);
				break;
			case 17:
				window.SetResolution(1920, 1200);
				break;
			case 18:
				window.SetResolution(2560, 1440);
				break;
			default:
				break;
			}
		}

		
		ImGui::End(); 
		
	}

	void OnEvent(Vi::Event& e) override {

		m_CameraControllerEX.OnEvent(e);

	
	}
	

private:

	Vi::ShaderLibrary m_ShaderLibrary;
	Vi::Ref<Vi::Shader> m_Shader;
	Vi::Ref<Vi::VertexArray> m_VertexArray;


	Vi::Ref<Vi::Shader> m_FlatColorShader;
	Vi::Ref<Vi::VertexArray> m_CubeVA;

	Vi::Ref<Vi::Texture2D> m_TextureEX, m_TransparentTextureEX;

	Vi::CameraController m_CameraControllerEX;


	glm::vec4 m_CubeColorEX = glm::vec4(0.5f, 0.2f, 0.2f, 1.0f);
	glm::vec4 m_GroundColorEX = glm::vec4(0.2f, 0.2f, 0.5f, 1.0f);

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
#endif

class Sandbox : public Vi::Application
{
public:
	Sandbox()
	{
	//	PushLayer(new ExampleLayer());
		PushLayer(new Sandbox3D());
	}
	~Sandbox()
	{
		
	}
};


Vi::Application* Vi::CreateApplication() {
	
	return new Sandbox();
}
