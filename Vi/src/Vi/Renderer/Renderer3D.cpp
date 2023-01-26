#include "Vipch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"


#include <glm/gtc/matrix_transform.hpp>

namespace Vi {

	struct Renderer3DStorage {

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexArray> PlaneVertexArray; 
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer3DStorage* s_Data;

	void Renderer3D::Init() {

		s_Data = new Renderer3DStorage();
		s_Data->CubeVertexArray = VertexArray::Create();

		//// Square VertexBuffer 
		//float cubeVertices[5 * 4 * 2] = {
		//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		//-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		// 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		//
		//-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		// 0.5f, -0.5f,  0.5f,  1.0f, 1.0f
		//};
		
		// Square VertexBuffer 
		float cubeVertices[ 5 * 4 * 6 ] = {
			// face 1
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

			// face 2
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,

			// face 3
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

			// face 4
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

			// face 5
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

			// face 6
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

		};
		Ref<VertexBuffer> cubeVB;
		cubeVB.reset(VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);

		// Cube IndexBuffer 
		uint32_t cubeIndices[ 6 * 6 ] = {

			0  ,  1  , 2   ,  2   , 3   , 0   ,
			4  ,  5  , 6   ,  6   , 7   , 4   ,
			8  ,  9  , 10  ,  10  , 11  , 8   ,
			12 , 13  , 14  ,  14  , 15  , 12  ,
			16 , 17  , 18  ,  18  , 19  , 16  ,
			20 , 21  , 22  ,  22  , 23  , 20

		};
		Ref<IndexBuffer> cubeIB;
		cubeIB.reset(IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t)));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);




		s_Data->PlaneVertexArray = VertexArray::Create();

		// Square VertexBuffer 
		float planeVertices[5 * 4] = {
			-0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
			-0.5f, 0.0f,  0.5f, 0.0f, 1.0f,
			 0.5f, 0.0f,  0.5f,	1.0f, 1.0f,
			 0.5f, 0.0f, -0.5f,	1.0f, 0.0f
		};

		Ref<VertexBuffer> planeVB;
		planeVB.reset(VertexBuffer::Create(planeVertices, sizeof(planeVertices)));
		planeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->PlaneVertexArray->AddVertexBuffer(planeVB);

		// Plane IndexBuffer 
		uint32_t planeIndices[6] = { 0,1,2,2,3,0 };

		Ref<IndexBuffer> planeIB;
		planeIB.reset(IndexBuffer::Create(planeIndices, sizeof(planeIndices) / sizeof(uint32_t)));
		s_Data->PlaneVertexArray->SetIndexBuffer(planeIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
		 
				
	}
	void Renderer3D::Shutdown() {
		
		delete s_Data;
	}

	void Renderer3D::BeginScene(const Camera& camera) {

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

	}

	void Renderer3D::EndScene() {

	}
	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color) {

		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform =	glm::translate(glm::mat4(1.0f), position) *
								glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f,0.0f,0.0f }) *
								glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f,1.0f,0.0f }) *
								glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f,0.0f,1.0f }) *
								glm::scale(glm::mat4(1.0f), scale);


		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color) {


		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);


		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}


	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color) {

		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		s_Data->TextureShader->SetMat4("u_Transform", transform);


		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
	}

	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color) {

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);


		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
					
	}

}