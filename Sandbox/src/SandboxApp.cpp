#include <Vi.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Vi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)/*, m_SquarePosition(0.0f)*/
	{

		LastMouseXPos = Vi::Input::GetMouseX();   // temp
		LastMouseYPos = Vi::Input::GetMouseY();   // temp

		m_VertexArray.reset(Vi::VertexArray::Create());


		/*	glGenVertexArrays(1, &m_VertexArray);
			glBindVertexArray(m_VertexArray);*/


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





		/// <summary>
		/// ////
		/// </summary>

		m_SquareVA.reset(Vi::VertexArray::Create());

		float squareVertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
		};

		std::shared_ptr<Vi::VertexBuffer> squareVB;
		squareVB.reset(Vi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));



		squareVB->SetLayout({ { Vi::ShaderDataType::Float3, "a_Position" } });
		m_SquareVA->AddVertexBuffer(squareVB);


		std::shared_ptr<Vi::IndexBuffer> squareIB;
		uint32_t squareIndices[6] = { 0,1,2, 2, 3, 0 };
		squareIB.reset(Vi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Vi::Shader(vertexSrc, fragmentSrc));

		/// <summary>
		/// ////
		/// </summary>
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

		m_FlatColorShader.reset(new Vi::Shader(flatColorVertexSrc, flatColorFragmentSrc));



	}

	virtual void OnUpdate(Vi::Timestep ts) override
	{

		//VI_INFO("DeltaTime: ({0}s) ({1}ms) FramesPerSecond ({2}FPS)", ts, ts.GetMilliSeconds(),1/ ts.GetSeconds());
		if (m_MouseButtonPressed)
		{
			float XOffset = Vi::Input::GetMouseX() - LastMouseXPos;             // temp
			float YOffset = Vi::Input::GetMouseY() - LastMouseYPos;				// temp
																			
			m_CameraPosition.x -= XOffset * 0.002f;								// temp
			m_CameraPosition.y += YOffset * 0.002f;								// temp
			LastMouseXPos += XOffset;											// temp
			LastMouseYPos += YOffset;											// temp
		}

		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();

		if (Vi::Input::IsKeyPressed(VI_KEY_W) || Vi::Input::IsKeyPressed(VI_KEY_UP)) { m_CameraPosition.y += m_CameraMoveSpeed * ts; }           // temp 
		if (Vi::Input::IsKeyPressed(VI_KEY_S) || Vi::Input::IsKeyPressed(VI_KEY_DOWN)) { m_CameraPosition.y -= m_CameraMoveSpeed * ts; }           // temp 
		if (Vi::Input::IsKeyPressed(VI_KEY_A) || Vi::Input::IsKeyPressed(VI_KEY_LEFT)) { m_CameraPosition.x -= m_CameraMoveSpeed * ts; }           // temp 
		if (Vi::Input::IsKeyPressed(VI_KEY_D) || Vi::Input::IsKeyPressed(VI_KEY_RIGHT)) { m_CameraPosition.x += m_CameraMoveSpeed * ts; }           // temp 
		if (Vi::Input::IsKeyPressed(VI_KEY_Q)) { m_CameraRotation -= m_CameraRotateSpeed * ts; }
		if (Vi::Input::IsKeyPressed(VI_KEY_E)) { m_CameraRotation += m_CameraRotateSpeed * ts; }
		

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
		
		

				
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(/*m_SquarePosition.x +*/ x * 0.11f , /*m_SquarePosition.y */+ y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale; //*glm::rotate(glm::mat4(1.0f), glm::radians(m_SquareRotation), glm::vec3(0, 0, 1));
				if (x % 2 == 0)
				{
					m_FlatColorShader->UploadUniformFloat4("u_Color",firstColor);
				}
				else
				{
					m_FlatColorShader->UploadUniformFloat4("u_Color", secondColor);
				}
				Vi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}


		Vi::Renderer::Submit(m_Shader, m_VertexArray);

		Vi::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Tab");														   // temp
		ImGui::SliderFloat4("ColorFlatChannel1", (float*)&firstColor, 0.0f, 1.0f);
		ImGui::SliderFloat4("ColorFlatChannel2", (float*)&secondColor, 0.0f, 1.0f);
		ImGui::End();																   // temp
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
				VI_TRACE("LastMouseXPos : {0} , LastMouseYPos : {1}", LastMouseXPos, LastMouseYPos);
			}
			VI_TRACE("{0}", (char)e.GetMouseButton());
			m_MouseButtonPressed = true;
		}																								 // temp

		if (event.GetEventType() == Vi::EventType::MouseButtonReleased)									 // temp
		{
			m_MouseButtonPressed = false;
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
	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 180.0f;


	glm::vec4 firstColor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 secondColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

	float LastMouseXPos;				// temp
	float LastMouseYPos;				// temp
	bool m_MouseButtonPressed = false;  // temp
	/*glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
	float m_SquareRotation = 0.0f;
	float m_SquareRotateSpeed = 180.0f;*/

};

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