#include <Vi.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Vi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(90.0f, 1600.0f/900.0f, 0.1f, 4000.0f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)/*, m_SquarePosition(0.0f)*/
	{
		m_Camera.SetPosition({ 0.0f,0.0f, 5.0f });
		m_Camera.SetRotation({ 0.0f,0.0f, 0.0f });
		LastMouseXPos = Vi::Input::GetMouseX();   // temp
		LastMouseYPos = Vi::Input::GetMouseY();   // temp

		m_VertexArray.reset(Vi::VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Vi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Vi::VertexBuffer::Create(vertices, sizeof(vertices)));
		Vi::BufferLayout layout = {
			{ Vi::ShaderDataType::Float3, "a_Position" },
			{ Vi::ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<Vi::IndexBuffer> indexBuffer;
		uint32_t indices[3] = { 0,1,2 };
		indexBuffer.reset(Vi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		
		m_SquareVA.reset(Vi::VertexArray::Create());

		float squareVertices[3 * 4 * 2] = {
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
					  
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f
					  
		};

		std::shared_ptr<Vi::VertexBuffer> squareVB;
		squareVB.reset(Vi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({ { Vi::ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<Vi::IndexBuffer> squareIB;
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

		m_Shader.reset(Vi::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Vi::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));
	}

	virtual void OnUpdate(Vi::Timestep ts) override
	{
		ExampleLayer::deltaTime = ts;										//temp


		//VI_INFO("DeltaTime: ({0}s) ({1}ms) FramesPerSecond ({2}FPS)", ts, ts.GetMilliSeconds(),1/ ts.GetSeconds());
		if (m_MouseMiddleButtonPressed)
		{
			float XOffset = Vi::Input::GetMouseX() - LastMouseXPos;             // temp
			float YOffset = Vi::Input::GetMouseY() - LastMouseYPos;				// temp
							
			
			m_CameraPosition.x -= XOffset * 0.2f * ts;								// temp
			m_CameraPosition.y += YOffset * 0.2f * ts;								// temp
			LastMouseXPos += XOffset;											// temp
			LastMouseYPos += YOffset;											// temp
		}

		if (m_MouseLeftButtonPressed)
		{
			float XOffset = Vi::Input::GetMouseX() - LastMouseXPos;             // temp
			float YOffset = Vi::Input::GetMouseY() - LastMouseYPos;				// temp

			float newYaw = m_CameraRotation.z - (XOffset * 45.0f * ts);
			float newPitch = m_CameraRotation.y - (YOffset * 45.0f * ts);

			m_CameraRotation.z = newYaw;								// temp
			
			if (newPitch > 89.0f )
			{
				newPitch =  89.0f;
				
			}
			else if (-89.0f > newPitch)
			{
				newPitch = -89.0f;
				
			} 
			
				m_CameraRotation.y = newPitch;
			
		

			/*(-89.0f < newPitch) ? (m_CameraRotation.y = YOffset * 180.5f * ts) : (m_CameraRotation.y = -89.0f);
			((YOffset * 180.5f * ts) < 89) ? (m_CameraRotation.y -= YOffset * 180.5f * ts) : (m_CameraRotation.y = 89.0f);*/
																					// temp
																					// temp
			
			LastMouseXPos += XOffset;											// temp
			LastMouseYPos += YOffset;											// temp
		}

		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();

	
		if (Vi::Input::IsKeyPressed(VI_KEY_W) || Vi::Input::IsKeyPressed(VI_KEY_UP)) 
			{ m_CameraPosition.z -= m_CameraMoveSpeed * ts; }           // temp 
		

		if (Vi::Input::IsKeyPressed(VI_KEY_S) || Vi::Input::IsKeyPressed(VI_KEY_DOWN)) 
			{ m_CameraPosition.z += m_CameraMoveSpeed * ts; }           // temp 

		//else { m_CameraPosition.z = 0.0f; }

		if (Vi::Input::IsKeyPressed(VI_KEY_A) || Vi::Input::IsKeyPressed(VI_KEY_LEFT)) 
			{ m_CameraPosition.x -= m_CameraMoveSpeed * ts; }           // temp 		

		if (Vi::Input::IsKeyPressed(VI_KEY_D) || Vi::Input::IsKeyPressed(VI_KEY_RIGHT))
			{ m_CameraPosition.x += m_CameraMoveSpeed * ts; }           // temp 
		//else { m_CameraPosition.x = 0.0f; }

		if (Vi::Input::IsKeyPressed(VI_KEY_Q)) 
			{ m_CameraPosition.y -= m_CameraMoveSpeed * ts; }
		
		if (Vi::Input::IsKeyPressed(VI_KEY_E))
			{ m_CameraPosition.y += m_CameraMoveSpeed * ts; }
		//else { m_CameraPosition.y = 0.0f; }

		
		

		//if (Vi::Input::IsKeyPressed(VI_KEY_I) || Vi::Input::IsKeyPressed(VI_KEY_UP))    { m_SquarePosition.y += m_SquareMoveSpeed * ts; }           // temp 
		//if (Vi::Input::IsKeyPressed(VI_KEY_K) || Vi::Input::IsKeyPressed(VI_KEY_DOWN))  { m_SquarePosition.y -= m_SquareMoveSpeed * ts; }           // temp 
		//if (Vi::Input::IsKeyPressed(VI_KEY_J) || Vi::Input::IsKeyPressed(VI_KEY_LEFT))  { m_SquarePosition.x -= m_SquareMoveSpeed * ts; }           // temp 
		//if (Vi::Input::IsKeyPressed(VI_KEY_L) || Vi::Input::IsKeyPressed(VI_KEY_RIGHT)) { m_SquarePosition.x += m_SquareMoveSpeed * ts; }
		//if (Vi::Input::IsKeyPressed(VI_KEY_U)) { m_SquareRotation -= m_SquareRotateSpeed * ts; }
		//if (Vi::Input::IsKeyPressed(VI_KEY_O)) { m_SquareRotation += m_SquareRotateSpeed * ts; }

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Vi::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		
		//Vi::MaterialRef material = new Vi::Material(m_FlatColorShader);        				// todo 
		//Vi::MaterialInstanceRef mi = new Vi::MaterialInstance(material);						// todo 
		
		//mi->SetValue("u_Color", firstColor);													// todo 
		//mi->SetTexture("u_Color", texture);													// todo 
		//mesh->SetMaterial(material);															// todo 


		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_ShaderColor);
				
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(/*m_SquarePosition.x +*/ x * 0.211f, 0.0f, /*m_SquarePosition.y */+ y * 0.211f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale; //*glm::rotate(glm::mat4(1.0f), glm::radians(m_SquareRotation), glm::vec3(0, 0, 1));
			
			
				Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}


	//	Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
	//	Vi::Renderer::Submit(mi, m_SquareVA, transform);			// todo 


		Vi::Renderer::Submit(m_Shader, m_VertexArray);

		Vi::Renderer::EndScene();

		if (m_CameraRotation.y > 89.0f || -89.0f > m_CameraRotation.y)
			VI_INFO("Camera Rotation Pitch is out of range {0}", m_Camera.GetRotation().y);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");														 // temp
		ImGui::ColorEdit4("ColorShader", (float*)&m_ShaderColor);

	//	ImGui::SliderFloat4("ColorFlatChannel1", (float*)&m_ShaderColor, 0.0f, 1.0f);
	//	ImGui::SliderFloat4("ColorFlatChannel2", (float*)&secondColor, 0.0f, 1.0f);
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

	void OnEvent(Vi::Event& event) override
	{
		if (event.GetEventType() == Vi::EventType::MouseButtonPressed)                                   // temp
		{
			LastMouseXPos = Vi::Input::GetMousePosition().first;
			LastMouseYPos = Vi::Input::GetMousePosition().second;
			Vi::MouseButtonPressedEvent& e = (Vi::MouseButtonPressedEvent&)event;
			
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_1)
			{
				m_MouseRightButtonPressed = true;
			}
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_2)
			{
				m_MouseLeftButtonPressed = true;
			}
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_MIDDLE)
			{
				m_MouseMiddleButtonPressed = true;
			}

		}																								 // temp

		if (event.GetEventType() == Vi::EventType::MouseButtonReleased)									 // temp
		{
			Vi::MouseButtonReleasedEvent& e = (Vi::MouseButtonReleasedEvent&)event;
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_1)
				m_MouseRightButtonPressed = false;
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_2)
				m_MouseLeftButtonPressed = false;
			if (e.GetMouseButton() == VI_MOUSE_BUTTON_3)
				m_MouseMiddleButtonPressed = false;
		}																								 // temp

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


	std::shared_ptr<Vi::Shader> m_Shader;
	std::shared_ptr<Vi::VertexArray> m_VertexArray;


	std::shared_ptr<Vi::Shader> m_FlatColorShader;
	std::shared_ptr<Vi::VertexArray> m_SquareVA;


	Vi::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	glm::vec3 m_CameraRotation;
	float m_CameraRotateSpeed = 180.0f;


	glm::vec4 m_ShaderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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


Vi::Application* Vi::CreateApplication()
{
	return new Sandbox();
}