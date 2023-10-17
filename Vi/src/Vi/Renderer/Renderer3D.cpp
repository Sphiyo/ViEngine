#include "Vipch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/quaternion.hpp>

namespace Vi {

	struct Renderer3DStorage {

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexArray> PlaneVertexArray; 
		Ref<Shader> TextureShader;
		Ref<Shader> SppotlightShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer3DStorage* s_Data;

	void Renderer3D::Init() {
		VI_PROFILE_FUNCTION();

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
		float cubeVertices[ 8 * 4 * 6] = {
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

			// Right face
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			 // Left face
			 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			 // Top face
			 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			 // Bottom face
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		};

		Ref<VertexBuffer> cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
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
		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);




		s_Data->PlaneVertexArray = VertexArray::Create();

		// Square VertexBuffer 
		float planeVertices[8 * 4] = {
			
			 // Top face
			-0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		Ref<VertexBuffer> planeVB = VertexBuffer::Create(planeVertices, sizeof(planeVertices));
		planeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->PlaneVertexArray->AddVertexBuffer(planeVB);

		// Plane IndexBuffer 
		uint32_t planeIndices[6] = { 0,1,2,2,3,0 };

		Ref<IndexBuffer> planeIB = IndexBuffer::Create(planeIndices, sizeof(planeIndices) / sizeof(uint32_t));
		s_Data->PlaneVertexArray->SetIndexBuffer(planeIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
		s_Data->TextureShader->Unbind();
		s_Data->SppotlightShader = Shader::Create("assets/shaders/SpotLightShader.glsl");
		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetInt("u_Texture", 0);
		s_Data->SppotlightShader->Unbind();
				
	}
	void Renderer3D::Shutdown() {
		VI_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer3D::BeginScene(const Camera& camera) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix()); 		
		s_Data->TextureShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data->TextureShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data->TextureShader->Unbind();

		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->SppotlightShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data->SppotlightShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data->SppotlightShader->Unbind();
	}

	void Renderer3D::EndScene() {
		VI_PROFILE_FUNCTION();

	}
	void Renderer3D::DrawCube(const glm::vec3& camPosition, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetFloat4("u_Color", color);
		s_Data->SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);

		s_Data->SppotlightShader->SetFloat3("cameraPos", camPosition);			// test
		s_Data->SppotlightShader->SetFloat("maxDistance", 10.0f);			// test

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		
		s_Data->SppotlightShader->SetMat4("model", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
		s_Data->SppotlightShader->Unbind();
	}
	void Renderer3D::DrawCube(const glm::vec3& camPosition, const glm::vec3& camDirection, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->SppotlightShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::toMat4(rotation) *
			glm::scale(glm::mat4(1.0f), scale);

		s_Data->SppotlightShader->SetFloat4("u_Color", color);
		s_Data->SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);
		// local vecs
		glm::vec3 m_ForwardVector;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;

		auto rot = glm::eulerAngles(rotation);
		m_ForwardVector.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
		m_ForwardVector.y = sin(glm::radians(rot.x));
		m_ForwardVector.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));

		m_ForwardVector = glm::normalize(m_ForwardVector);
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector)); 
		auto posLight = position;
		posLight.y -= 1.0f;
		auto dirlight = glm::vec3(0.5f, -1.0f, 0.0f);
		glm::normalize(dirlight);
		s_Data->SppotlightShader->SetFloat3("lightPos", posLight);				// test
		s_Data->SppotlightShader->SetFloat3("lightDir", dirlight);				// test
		s_Data->SppotlightShader->SetFloat("cosInnerCone", glm::cos(glm::radians(30.0f)));	// test
		s_Data->SppotlightShader->SetFloat("cosOuterCone", glm::cos(glm::radians(45.0f)));	// test
		s_Data->SppotlightShader->SetFloat("maxDistance", 1000.0f);					// test
		s_Data->SppotlightShader->SetFloat("innerConstantAttenuation", 1.0f);					// test
		s_Data->SppotlightShader->SetFloat("outerConstantAttenuation", 1.0f);					// test
		s_Data->SppotlightShader->SetFloat("innerLinearAttenuation", 0.1f);					// test
		s_Data->SppotlightShader->SetFloat("outerLinearAttenuation", 0.1f);					// test
		s_Data->SppotlightShader->SetFloat("innerQuadraticAttenuation", 0.031f);					// test
		s_Data->SppotlightShader->SetFloat("outerQuadraticAttenuation", 0.031f);					// test
		s_Data->SppotlightShader->SetFloat("shininess", 300.0f);					// test
		s_Data->SppotlightShader->SetFloat3("lightColor", { 1.0f, 1.0f, 1.0f });
	//	s_Data->SppotlightShader->SetFloat3("lightColor", color.xyz);					// test
		s_Data->SppotlightShader->SetFloat3("specularColor", {0.2f, 0.0f, 1.0f});					// test

		s_Data->WhiteTexture->Bind();
	
	
		
		//s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->SppotlightShader->SetMat4("model", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
		s_Data->SppotlightShader->Unbind();
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->SppotlightShader->SetFloat4("u_Color", color);
		s_Data->SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data->SppotlightShader->SetMat4("u_Transform", transform);

		s_Data->SppotlightShader->SetMat4("model", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}


	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		//s_Data->TextureShader->SetMat4("model", transform);

		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
		s_Data->TextureShader->Unbind();
	}

	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data->TextureShader->SetMat4("u_Transform", transform);


		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
		s_Data->TextureShader->Unbind();
	}

}