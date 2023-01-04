#include <Vi.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Vi::Layer {

public:
	ExampleLayer()
		: Layer("Example") , m_Camera(90.0f, 800.0f/600.0f, 0.1f, 4000.0f) {

		m_Camera.SetPosition({ 0.0f,0.0f, 5.0f });
		m_Camera.SetRotation({ 0.0f,0.0f, 0.0f });
		LastMouseXPos = Vi::Input::GetMouseX();   // temp
		LastMouseYPos = Vi::Input::GetMouseY();   // temp

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
		m_Shader.reset(Vi::Shader::Create(vertexSrc, fragmentSrc));

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
		m_FlatColorShader.reset(Vi::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));


		// Create And Compile texture Shader 
		m_TextureShader.reset(Vi::Shader::Create("assets/shaders/Texture.glsl"));

		//Construct And Buffer Texture To Memory
		m_Texture = Vi::Texture2D::Create("assets/textures/Cat03.jpg", 0);
		m_TransparentTexture = Vi::Texture2D::Create("assets/textures/RGBA.png", 0);

		// bind texture Shader And Set Uniform
		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	virtual void OnUpdate(Vi::Timestep ts) override	{

		ExampleLayer::deltaTime = ts;										//temp
		
		if (m_MouseMiddleButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMouseXPos;             // temp
			float YOffset = Vi::Input::GetMouseY() - LastMouseYPos;				// temp
			m_CameraPosition.x -= XOffset * 0.2f * ts;								// temp
			m_CameraPosition.y += YOffset * 0.2f * ts;								// temp
			LastMouseXPos += XOffset;											// temp
			LastMouseYPos += YOffset;											// temp
		}

		if (m_MouseLeftButtonPressed) {

			float XOffset = Vi::Input::GetMouseX() - LastMouseXPos;             // temp
			float YOffset = Vi::Input::GetMouseY() - LastMouseYPos;				// temp
			VI_INFO( "RotationYaw : {0} ", m_CameraRotation.z);
			float newYaw = m_CameraRotation.z - (XOffset * 45.0f * ts);
			float newPitch = m_CameraRotation.y - (YOffset * 45.0f * ts);
			m_CameraRotation.z = newYaw;								// temp
			if (newPitch > 89.0f) { newPitch = 89.0f; }
			else if (-89.0f > newPitch) { newPitch = -89.0f; }
			m_CameraRotation.y = newPitch;
			LastMouseXPos += XOffset;											// temp
			LastMouseYPos += YOffset;											// temp
		}
		// Set Background Color And Clear It 
		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();
	
		if (Vi::Input::IsKeyPressed(VI_KEY_W) || Vi::Input::IsKeyPressed(VI_KEY_UP)) {
			m_CameraPosition.z -= m_CameraMoveSpeed * ts; 
		}           // temp 

		if (Vi::Input::IsKeyPressed(VI_KEY_S) || Vi::Input::IsKeyPressed(VI_KEY_DOWN)) {
			m_CameraPosition.z += m_CameraMoveSpeed * ts;
		}           // temp 

		if (Vi::Input::IsKeyPressed(VI_KEY_A) || Vi::Input::IsKeyPressed(VI_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}           // temp 		

		if (Vi::Input::IsKeyPressed(VI_KEY_D) || Vi::Input::IsKeyPressed(VI_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}           // temp 
	
		if (Vi::Input::IsKeyPressed(VI_KEY_Q)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		
		if (Vi::Input::IsKeyPressed(VI_KEY_E)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		
		Vi::Renderer::BeginScene(m_Camera);
		
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
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				
				glm::vec3 pos(x * 0.211f, 0.0f, y * 0.211f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				m_Texture->Bind();
				Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
	 
	//	Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
	//	Vi::Renderer::Submit(mi, m_SquareVA, transform);			// todo 
	
		// Draw Cube With Texture 
		m_Texture->Bind();
		Vi::Renderer::Submit(m_TextureShader, m_SquareVA);
		
		// Draw Cube With Alpha Blend Texture 
		m_TransparentTexture->Bind();
		Vi::Renderer::Submit(m_TextureShader, m_SquareVA);
	
		
		Vi::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override {

		ImGui::Begin("Settings");														 // temp
		ImGui::ColorEdit4("ColorShader", (float*)&m_ShaderColor);
		ImGui::End();																   // temp
		
		ImGui::Begin("Stats");														   // temp 
		ImGui::Text("Fps  %.0fFps", 1/ ExampleLayer::deltaTime);
		ImGui::Text("ts  %.3fms" , ExampleLayer::deltaTime);
		ImGui::End();

		ImGui::Begin("GraphicsSetting");		
		ImGui::Checkbox("Vsync", &vSync);
		Vi::Application::Get().GetWindow().SetVSync(vSync);
		ImGui::End();
	}

	void OnEvent(Vi::Event& event) override {

		if (event.GetEventType() == Vi::EventType::MouseButtonPressed) {
			LastMouseXPos = Vi::Input::GetMousePosition().first;
			LastMouseYPos = Vi::Input::GetMousePosition().second;
			Vi::MouseButtonPressedEvent& e = (Vi::MouseButtonPressedEvent&)event;
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_1) { m_MouseRightButtonPressed = true; }
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_2) { m_MouseLeftButtonPressed = true; }
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE){ m_MouseMiddleButtonPressed = true; }
		}																								 // temp

		if (event.GetEventType() == Vi::EventType::MouseButtonReleased) {
			Vi::MouseButtonReleasedEvent& e = (Vi::MouseButtonReleasedEvent&)event;
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_1) { m_MouseRightButtonPressed = false; }
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_2) { m_MouseLeftButtonPressed = false; }
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_3) { m_MouseMiddleButtonPressed = false; }																							 // temp
		}
		if (event.GetEventType() == Vi::EventType::MouseScrolled)									 // temp
		{
			Vi::MouseScrolledEvent& e = (Vi::MouseScrolledEvent&)event;
			m_CameraPosition.z -= e.GetYOffset();
			m_Camera.SetPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().y + 10000.0f + e.GetYOffset() });
		//	m_Camera.SetPosition({ m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().y + 10000.0f + e.GetYOffset()});
			VI_WARN("scrollmouseevent {}" ,e.GetYOffset());
			
		}																								 // temp

	}
	

private:


	Vi::Ref<Vi::Shader> m_Shader;
	Vi::Ref<Vi::VertexArray> m_VertexArray;


	Vi::Ref<Vi::Shader> m_FlatColorShader , m_TextureShader;
	Vi::Ref<Vi::VertexArray> m_SquareVA;

	Vi::Ref<Vi::Texture2D> m_Texture, m_TransparentTexture;

	Vi::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition = glm::vec3(0.0f);;
	float m_CameraMoveSpeed = 5.0f;
	glm::vec3 m_CameraRotation = glm::vec3(0.0f);
	float m_CameraRotateSpeed = 180.0f;


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
