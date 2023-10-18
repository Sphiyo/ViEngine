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

	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		//glm::vec4 Color;
		glm::vec2 TexCoord;
		//	float TexIndex;
		//	float TilingFactor;
	};

	struct Renderer3DData {

		static const uint32_t CubeVertices = 4 * 6;
		static const uint32_t CubeIndices = 6 * 6;

		const uint32_t MaxCubes = 5'000;
		const uint32_t MaxCubeVertices = MaxCubes * CubeVertices;
		const uint32_t MaxCubeIndices = MaxCubes * CubeIndices;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<IndexBuffer> CubeIndexBuffer;


		static const uint32_t PlaneVertices = 4;
		static const uint32_t PlaneIndices = 6;

		const uint32_t MaxPlanes = 10'000;
		const uint32_t MaxPlaneVertices = MaxPlanes * PlaneVertices;
		const uint32_t MaxPlaneIndices = MaxPlanes * PlaneIndices;

		Ref<VertexArray>  PlaneVertexArray;
		Ref<VertexBuffer> PlaneVertexBuffer;
		Ref<IndexBuffer>  PlaneIndexBuffer;


		static const uint32_t MaxTextureSlots = 32;

		Ref<Shader> TextureShader;
		Ref<Shader> SpotlightShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		uint32_t PlaneIndexCount = 0;

		Vertex* CubeVertexBufferBase = nullptr;
		Vertex* CubeVertexBufferPtr = nullptr;

		Vertex* PlaneVertexBufferBase = nullptr;
		Vertex* PlaneVertexBufferPtr = nullptr;

		std::array<uint32_t, MaxTextureSlots> TextureSlot;
		uint32_t TextureSlotIndex = 1; // 0 for white texture
	};

	static Renderer3DData s_Data3D;

	void Renderer3D::Init() {
		VI_PROFILE_FUNCTION();

		s_Data3D.CubeVertexArray = VertexArray::Create();

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
	/*	float cubeVertices[8 * 4 * 6] = {
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
		*/
		s_Data3D.CubeVertexBuffer = VertexBuffer::Create(s_Data3D.MaxCubeVertices * sizeof(Vertex));
		s_Data3D.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data3D.CubeVertexArray->AddVertexBuffer(s_Data3D.CubeVertexBuffer);

		s_Data3D.CubeVertexBufferBase = new Vertex[s_Data3D.MaxCubeVertices];

		uint32_t* cubeIndices = new uint32_t[s_Data3D.MaxCubeIndices];

		uint32_t CIndices[s_Data3D.CubeIndices] = {

			0  ,  1  , 2   ,  2   , 3   , 0   ,
			4  ,  5  , 6   ,  6   , 7   , 4   ,
			8  ,  9  , 10  ,  10  , 11  , 8   ,
			12 , 13  , 14  ,  14  , 15  , 12  ,
			16 , 17  , 18  ,  18  , 19  , 16  ,
			20 , 21  , 22  ,  22  , 23  , 20
		};

		uint32_t offset = 0;

		// i = indices
		for (uint32_t i = 0; i < s_Data3D.MaxCubeIndices; i++) {
			uint32_t DynamicIndices = i % s_Data3D.CubeIndices;
			cubeIndices[i] = offset + CIndices[DynamicIndices];
			if ((DynamicIndices + 1) == s_Data3D.CubeIndices)
				offset += s_Data3D.CubeVertices;
		}

		s_Data3D.CubeIndexBuffer = IndexBuffer::Create(cubeIndices, s_Data3D.MaxCubeIndices);
		s_Data3D.CubeVertexArray->SetIndexBuffer(s_Data3D.CubeIndexBuffer);

		delete[] cubeIndices;

		s_Data3D.CubeVertexArray->Unbind();
		s_Data3D.CubeVertexBuffer->Unbind();
		s_Data3D.CubeIndexBuffer->Unbind();


		s_Data3D.PlaneVertexArray = VertexArray::Create();

		// Square VertexBuffer 
		float planeVertices[8 * 4] = {

			// Top face
		   -0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		   -0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		s_Data3D.PlaneVertexBuffer = VertexBuffer::Create(s_Data3D.MaxPlaneVertices * sizeof(Vertex));
		s_Data3D.PlaneVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data3D.PlaneVertexArray->AddVertexBuffer(s_Data3D.PlaneVertexBuffer);

		s_Data3D.PlaneVertexBufferBase = new Vertex[s_Data3D.MaxPlaneVertices];

		// to do make ref count for thread safety if the func got cued for longer time 
		uint32_t* planeIndices = new uint32_t[s_Data3D.MaxPlaneIndices];

		uint32_t PIndices[s_Data3D.PlaneIndices] = { 0, 1, 2, 2, 3, 0 };

		offset = 0;

		// i = indices
		for (uint32_t i = 0; i < s_Data3D.MaxPlaneIndices; i++) {
			uint32_t DynamicIndices = i % s_Data3D.PlaneIndices;
			planeIndices[i] = offset + PIndices[DynamicIndices];
			if ((DynamicIndices + 1) == s_Data3D.PlaneIndices)
				offset += s_Data3D.PlaneVertices;
		}

		s_Data3D.PlaneIndexBuffer = IndexBuffer::Create(planeIndices, s_Data3D.MaxPlaneIndices);
		s_Data3D.PlaneVertexArray->SetIndexBuffer(s_Data3D.PlaneIndexBuffer);

		delete[] planeIndices;

		s_Data3D.PlaneVertexArray->Unbind();
		s_Data3D.PlaneVertexBuffer->Unbind();
		s_Data3D.PlaneIndexBuffer->Unbind();


		s_Data3D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data3D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_Data3D.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetInt("u_Texture", 0);
		s_Data3D.TextureShader->Unbind();
		s_Data3D.SpotlightShader = Shader::Create("assets/shaders/SpotLightShader.glsl");
		s_Data3D.SpotlightShader->Bind();
		s_Data3D.SpotlightShader->SetInt("u_Texture", 0);
		s_Data3D.SpotlightShader->Unbind();

	}
	void Renderer3D::Shutdown() {
		VI_PROFILE_FUNCTION();

		
	}

	void Renderer3D::BeginScene(const Camera3D& camera) {
		VI_PROFILE_FUNCTION();

		int CubeInstanceCount = s_Data3D.CubeIndexCount / s_Data3D.CubeIndices;
		int PlaneInstanceCount = s_Data3D.PlaneIndexCount / s_Data3D.PlaneIndices;

		VI_CORE_INFO("Cube Instance Count : {}", CubeInstanceCount);
		VI_CORE_INFO("Plane Instance Count : {}", PlaneInstanceCount);
		VI_CORE_INFO("Total Cube Indices Count : {}", (s_Data3D.CubeIndexCount));
		VI_CORE_INFO("Total Plane Indices Count : {}", (s_Data3D.PlaneIndexCount));
		VI_CORE_INFO("Total Cube Vertex Count : {}", (CubeInstanceCount * s_Data3D.CubeIndices));
		VI_CORE_INFO("Total Plane Vertex Count : {}", (PlaneInstanceCount * s_Data3D.PlaneIndices));

		s_Data3D.CubeIndexCount = 0;
		s_Data3D.PlaneIndexCount = 0;

		s_Data3D.CubeVertexBufferPtr = s_Data3D.CubeVertexBufferBase;
		s_Data3D.PlaneVertexBufferPtr = s_Data3D.PlaneVertexBufferBase;

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data3D.TextureShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data3D.TextureShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data3D.TextureShader->Unbind();


		s_Data3D.SpotlightShader->Bind();
		s_Data3D.SpotlightShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data3D.SpotlightShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data3D.SpotlightShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data3D.SpotlightShader->Unbind();


	}

	void Renderer3D::EndScene() {
		VI_PROFILE_FUNCTION()

			//	s_Data3D.CubeVertexArray->Bind();
			//	s_Data3D.CubeVertexBuffer->Bind();
			//	s_Data3D.CubeIndexBuffer->Bind();



			Flush();
	}

	void Renderer3D::Flush() {
		//VI_PROFILE_FUNCTION();
		s_Data3D.TextureShader->Bind();
		s_Data3D.CubeVertexArray->Bind();
		uint32_t CubeDataSize = (uint32_t)((uint8_t*)s_Data3D.CubeVertexBufferPtr - (uint8_t*)s_Data3D.CubeVertexBufferBase);
		s_Data3D.CubeVertexBuffer->SetData(s_Data3D.CubeVertexBufferBase, CubeDataSize);

		RenderCommand::DrawIndexed(s_Data3D.CubeVertexArray, s_Data3D.CubeIndexCount);

		s_Data3D.PlaneVertexArray->Bind();
		uint32_t PlaneDataSize = (uint32_t)((uint8_t*)s_Data3D.PlaneVertexBufferPtr - (uint8_t*)s_Data3D.PlaneVertexBufferBase);
		s_Data3D.PlaneVertexBuffer->SetData(s_Data3D.PlaneVertexBufferBase, PlaneDataSize);

		RenderCommand::DrawIndexed(s_Data3D.PlaneVertexArray, s_Data3D.PlaneIndexCount);

		//s_Data3D.CubeVertexArray->Unbind();
		//s_Data3D.CubeVertexBuffer->Unbind();
		//s_Data3D.CubeIndexBuffer->Unbind();
		//
		//s_Data3D.PlaneVertexArray->Unbind();
		//s_Data3D.PlaneVertexBuffer->Unbind();
		//s_Data3D.PlaneIndexBuffer->Unbind();

		s_Data3D.TextureShader->Unbind();
		VI_CORE_INFO("RendererAPI::GetDrawCallsCount = {}", RendererAPI::GetDrawCallsCount());
	}

	void Renderer3D::DrawCube(const glm::vec3& camPosition, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetFloat4("u_Color", color);

		constexpr glm::vec3 Positions[ 4 * 6] =
		{
			// face 1	
			{  -0.5f, -0.5f, -0.5f   },
			{   0.5f, -0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },

			// face 2			   //
			{  -0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{  -0.5f,  0.5f,  0.5f   },


			// face 3			   //
			{  -0.5f, -0.5f,  0.5f   },
			{  -0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },


			// face 4			   //
			{   0.5f, -0.5f, -0.5f   },
			{   0.5f, -0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f, -0.5f   },


			// face 5			   //
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f, -0.5f   },


			// face 6			   //
			{  -0.5f, -0.5f,  0.5f   },
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f,  0.5f   }

		};
		constexpr glm::vec3 Normals[4 * 6] =
		{
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },
			  					 
			  					 
			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },
			  					 
			  					 
			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },
						   		 
						   		 
			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },
							   	 
						   		 
			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },
			 			   		 
			 		   			 
			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f }
		
		};

		constexpr glm::vec2 textureCoords[4 * 6] =
		{

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },
		

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },
			

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },
			

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },
			

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },
			

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f }

		};

	 

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		glm::vec4 pos;
		for (size_t i = 0; i < s_Data3D.CubeVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
			//	s_Data3D.CubeVertexBufferPtr->Position =  position + Positions[i] ; // working
			s_Data3D.CubeVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
			s_Data3D.CubeVertexBufferPtr->Normal = Normals[i];
			s_Data3D.CubeVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_Data3D.CubeVertexBufferPtr->TexIndex = textureIndex;
			//	s_Data3D.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data3D.CubeVertexBufferPtr++;
		}
		s_Data3D.CubeIndexCount += s_Data3D.CubeIndices;
	}
	void Renderer3D::DrawCube(const glm::vec3& camPosition, const glm::vec3& camDirection, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetFloat4("u_Color", color);
		s_Data3D.WhiteTexture->Bind();
		constexpr glm::vec3 Positions[4 * 6] =
		{
			// face 1	
			{  -0.5f, -0.5f, -0.5f   },
			{   0.5f, -0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },

			// face 2			   //
			{  -0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{  -0.5f,  0.5f,  0.5f   },


			// face 3			   //
			{  -0.5f, -0.5f,  0.5f   },
			{  -0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },


			// face 4			   //
			{   0.5f, -0.5f, -0.5f   },
			{   0.5f, -0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f, -0.5f   },


			// face 5			   //
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f, -0.5f   },


			// face 6			   //
			{  -0.5f, -0.5f,  0.5f   },
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f,  0.5f   }

		};
		constexpr glm::vec3 Normals[4 * 6] =
		{
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },
			{ 0.0f,  0.0f,  1.0f },


			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },
			{ 0.0f,  0.0f, -1.0f },


			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },
			{ 1.0f,  0.0f,  0.0f },


			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },
			{-1.0f,  0.0f,  0.0f },


			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },
			{ 0.0f,  1.0f,  0.0f },


			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f },
			{ 0.0f, -1.0f,  0.0f }

		};

		constexpr glm::vec2 textureCoords[4 * 6] =
		{

			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },


			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },


			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },


			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },


			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f },


			{ 0.0f,  0.0f },
			{ 1.0f,  0.0f },
			{ 1.0f,  1.0f },
			{ 0.0f,  1.0f }

		};



		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::toMat4(rotation) *
			glm::scale(glm::mat4(1.0f), scale);

		glm::vec4 pos;
		for (size_t i = 0; i < s_Data3D.CubeVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
			//	s_Data3D.CubeVertexBufferPtr->Position =  position + Positions[i] ; // working
			s_Data3D.CubeVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
			s_Data3D.CubeVertexBufferPtr->Normal = Normals[i];
			s_Data3D.CubeVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_Data3D.CubeVertexBufferPtr->TexIndex = textureIndex;
			//	s_Data3D.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data3D.CubeVertexBufferPtr++;
		}
		s_Data3D.CubeIndexCount += s_Data3D.CubeIndices;
		s_Data3D.TextureShader->SetMat4("u_Transform", transform);
		//s_Data3D.SppotlightShader->SetMat4("model", transform);
		//		VI_PROFILE_FUNCTION();
//
//		s_Data3D.SppotlightShader->Bind();
//
//		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
//			glm::toMat4(rotation) *
//			glm::scale(glm::mat4(1.0f), scale);
//
//		s_Data3D.SppotlightShader->SetFloat4("u_Color", color);
//		s_Data3D.SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);
//		// local vecs
//		glm::vec3 m_ForwardVector;
//		glm::vec3 m_RightVector;
//		glm::vec3 m_UpVector;
//
//		auto rot = glm::eulerAngles(rotation);
//		m_ForwardVector.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
//		m_ForwardVector.y = sin(glm::radians(rot.x));
//		m_ForwardVector.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
//
//		m_ForwardVector = glm::normalize(m_ForwardVector);
//		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
//
//		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));
//		auto posLight = position;
//		posLight.y -= 1.0f;
//		auto dirlight = glm::vec3(0.5f, -1.0f, 0.0f);
//		glm::normalize(dirlight);
//		s_Data3D.SppotlightShader->SetFloat3("lightPos", posLight);				// test
//		s_Data3D.SppotlightShader->SetFloat3("lightDir", dirlight);				// test
//		s_Data3D.SppotlightShader->SetFloat("cosInnerCone", glm::cos(glm::radians(30.0f)));	// test
//		s_Data3D.SppotlightShader->SetFloat("cosOuterCone", glm::cos(glm::radians(45.0f)));	// test
//		s_Data3D.SppotlightShader->SetFloat("maxDistance", 1000.0f);					// test
//		s_Data3D.SppotlightShader->SetFloat("innerConstantAttenuation", 1.0f);					// test
//		s_Data3D.SppotlightShader->SetFloat("outerConstantAttenuation", 1.0f);					// test
//		s_Data3D.SppotlightShader->SetFloat("innerLinearAttenuation", 0.1f);					// test
//		s_Data3D.SppotlightShader->SetFloat("outerLinearAttenuation", 0.1f);					// test
//		s_Data3D.SppotlightShader->SetFloat("innerQuadraticAttenuation", 0.031f);					// test
//		s_Data3D.SppotlightShader->SetFloat("outerQuadraticAttenuation", 0.031f);					// test
//		s_Data3D.SppotlightShader->SetFloat("shininess", 300.0f);					// test
//		s_Data3D.SppotlightShader->SetFloat3("lightColor", { 1.0f, 1.0f, 1.0f });
//		//	s_Data3D.SppotlightShader->SetFloat3("lightColor", color.xyz);					// test
//		s_Data3D.SppotlightShader->SetFloat3("specularColor", { 0.2f, 0.0f, 1.0f });					// test
// 
//
// 
//
//		s_Data3D.SpotlightShader->SetMat4("model", transform);
//		for (size_t i = 0; i < cubeVertexCount; i++)  // test // return here // to do //experimental
//		{
//			glm::vec4 pos = transform * glm::vec4{ Positions[i], 0.0f };
//			for (size_t i = 0; i < s_Data.CubeVertices; i++) {
//				glm::vec4 pos = transform * glm::vec4{ Positions[i], 0.0f };
//				//	s_Data.CubeVertexBufferPtr->Position =  position + Positions[i] ; // working
//				s_Data.CubeVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
//				s_Data.CubeVertexBufferPtr->Color = color;
//				s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
//				//	s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
//				//	s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
//				s_Data.CubeVertexBufferPtr++;
//				glm::vec4 pos;
//				for (size_t i = 0; i < s_Data3D.CubeVertices; i++) {
//					pos = transform * glm::vec4{ Positions[i], 0.0f };
//					//	s_Data3D.CubeVertexBufferPtr->Position =  position + Positions[i] ; // working
//					s_Data3D.CubeVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
//					s_Data3D.CubeVertexBufferPtr->Color = color;
//					s_Data3D.CubeVertexBufferPtr->TexCoord = textureCoords[i];
//					//	s_Data3D.CubeVertexBufferPtr->TexIndex = textureIndex;
//					//	s_Data3D.CubeVertexBufferPtr->TilingFactor = tilingFactor;
//					s_Data3D.CubeVertexBufferPtr++;
//				}
//				s_Data3D.CubeIndexCount += s_Data3D.CubeIndices;
			}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
//		VI_PROFILE_FUNCTION();
//
//		s_Data3D.SppotlightShader->SetFloat4("u_Color", color);
//		s_Data3D.SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);
//
//		texture->Bind();
//
//		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
//			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
//			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
//			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
//			glm::scale(glm::mat4(1.0f), scale);
//
//		s_Data3D.SppotlightShader->SetMat4("u_Transform", transform);
//
//		s_Data3D.SppotlightShader->SetMat4("model", transform);
//
//		s_Data3D.CubeVertexArray->Bind();
//		RenderCommand::DrawIndexed(s_Data3D.CubeVertexArray);
	}


	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetFloat4("u_Color", color);
		s_Data3D.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		s_Data3D.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data3D.TextureShader->SetMat4("u_Transform", transform);

		//s_Data3D.TextureShader->SetMat4("model", transform);

		s_Data3D.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data3D.PlaneVertexArray);
		s_Data3D.TextureShader->Unbind();
	}

	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetFloat4("u_Color", tint);
		s_Data3D.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data3D.TextureShader->SetMat4("u_Transform", transform);


		s_Data3D.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data3D.PlaneVertexArray);
		s_Data3D.TextureShader->Unbind();
	}

}