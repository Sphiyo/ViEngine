#include "Vipch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Vi {

	struct Renderer3DStorage {

		Ref<VertexArray> CubeVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer3DStorage* s_Data;

	void Renderer3D::Init() {

		s_Data = new Renderer3DStorage();
		s_Data->CubeVertexArray = VertexArray::Create();

		// Square VertexBuffer 
		float cubeVertices[5 * 4 * 2] = {
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f
		};
		Ref<VertexBuffer> cubeVB;
		cubeVB.reset(VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);

		// Cube IndexBuffer 
		uint32_t cubeIndices[6 * 6] = {
			0,1,2,2,3,0,
			0,4,7,7,3,0,
			4,5,6,6,7,4,
			5,1,2,2,6,5,
			3,2,6,6,7,3,
			0,1,5,5,4,0
		};
		Ref<IndexBuffer> cubeIB;
		cubeIB.reset(IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t)));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);


		// Create And Compile flat Shader 
		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");

	}
	void Renderer3D::Shutdown() {
		
		delete s_Data;
	}

	void Renderer3D::BeginScene(const Camera& camera) {

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer3D::EndScene() {

	}
	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}
}