#include "Vipch.h"
#include "RendererUI.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"


#include <glm/gtc/matrix_transform.hpp>


//temp
#include "Vi/Miscs/CameraController2D.h"



namespace Vi {

	struct PlaneVertex {

		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//	float TexIndex;
		//	float TilingFactor;
	};

	struct RendererUIData {
		const uint32_t PlaneVertices = 4;
		const uint32_t PlaneIndices = 6;

		const uint32_t MaxPlane = 5'000;
		const uint32_t MaxVertices = MaxPlane * PlaneVertices;
		const uint32_t MaxIndices = MaxPlane * PlaneIndices;

		 
		Ref<VertexArray> PlaneVertexArray;
		Ref<VertexBuffer> PlaneVertexBuffer;
		Ref<IndexBuffer> PlaneIndexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t PlaneIndexCount = 0;
		PlaneVertex* PlaneVertexBufferBase = nullptr;
		PlaneVertex* PlaneVertexBufferPtr = nullptr;

		//		std::array<uint32_t> 
	};

	static RendererUIData s_DataUI;


	void RendererUI::Init() {

		VI_PROFILE_FUNCTION();

		s_DataUI.PlaneVertexArray = VertexArray::Create();


		s_DataUI.PlaneVertexBuffer = VertexBuffer::Create(s_DataUI.MaxVertices * sizeof(PlaneVertex));


		s_DataUI.PlaneVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },    // TODO
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_DataUI.PlaneVertexArray->AddVertexBuffer(s_DataUI.PlaneVertexBuffer);


		s_DataUI.PlaneVertexBufferBase = new PlaneVertex[s_DataUI.MaxVertices];

		// to do make ref count for thread safety if the func got cued for longer time 
		
		uint32_t* planeIndices = new uint32_t[s_DataUI.MaxIndices];

		uint32_t Indices[6] = { 0, 1, 2, 2, 3, 0 };

		uint32_t offset = 0;

		// i = indices
		for (uint32_t i = 0; i < s_DataUI.MaxIndices; i++) {
			uint32_t DynamicIndices = i % s_DataUI.PlaneIndices;
			planeIndices[i] = offset + Indices[DynamicIndices];
			if ((DynamicIndices + 1) == s_DataUI.PlaneIndices)
				offset += s_DataUI.PlaneVertices;
		}
		
		s_DataUI.PlaneIndexBuffer = IndexBuffer::Create(planeIndices,s_DataUI.MaxIndices);
	//	Ref<IndexBuffer> cubeIB = IndexBuffer::Create(planeIndices, s_DataUI.MaxIndices);
		s_DataUI.PlaneVertexArray->SetIndexBuffer(s_DataUI.PlaneIndexBuffer);

		delete[] planeIndices;
		

		s_DataUI.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_DataUI.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_DataUI.TextureShader = Shader::Create("assets/shaders/UI.glsl");

		s_DataUI.TextureShader->Bind();
		s_DataUI.TextureShader->SetInt("u_Texture", 0);

		s_DataUI.TextureShader->Unbind();
		s_DataUI.PlaneVertexArray->Unbind();
		s_DataUI.PlaneVertexBuffer->Unbind();
		s_DataUI.PlaneIndexBuffer->Unbind();

	}
	void RendererUI::Shutdown() {
		VI_PROFILE_FUNCTION();


	}

	void RendererUI::BeginScene(const Camera2D& camera) {
		VI_PROFILE_FUNCTION();


		s_DataUI.PlaneVertexArray->Bind();
		s_DataUI.PlaneVertexBuffer->Bind();
		s_DataUI.PlaneIndexBuffer->Bind();
		s_DataUI.TextureShader->Bind();
	//	VI_CORE_WARN("(RendererUI.cpp) : ViewProj = {}", camera.GetViewProjectionMatrix());
		
		s_DataUI.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		

		int PlaneInstanceCount = s_DataUI.PlaneIndexCount / s_DataUI.PlaneIndices;
		 
		VI_CORE_INFO("Plane Instance Count : {}", PlaneInstanceCount); 

		VI_CORE_INFO("Total Indices Count : {}", (s_DataUI.PlaneIndexCount));
		//	VI_CORE_INFO( "Total Vertex Count : {}", ( s_DataUI.PlaneIndexCount / s_DataUI.PlaneIndices ));
		//	s_DataUI.PlaneIndexCount %= s_DataUI.CubeIndices; 
		s_DataUI.PlaneIndexCount = 0;

		s_DataUI.PlaneVertexBufferPtr = s_DataUI.PlaneVertexBufferBase;

	}


	void RendererUI::EndScene() {
		VI_PROFILE_FUNCTION();
	//	s_DataUI.PlaneVertexArray->Bind();
	//	s_DataUI.PlaneVertexBuffer->Bind();
	//	s_DataUI.PlaneIndexBuffer->Bind();

		uint32_t datasize = (uint32_t)((uint8_t*)s_DataUI.PlaneVertexBufferPtr - (uint8_t*)s_DataUI.PlaneVertexBufferBase);
		s_DataUI.PlaneVertexBuffer->SetData(s_DataUI.PlaneVertexBufferBase, datasize);


		Flush();
	}

	void RendererUI::Flush() {
		//VI_PROFILE_FUNCTION();
		s_DataUI.TextureShader->Bind();
		RenderCommand::DrawIndexed(s_DataUI.PlaneVertexArray, s_DataUI.PlaneIndexCount);

		s_DataUI.PlaneVertexArray->Unbind();
		s_DataUI.PlaneVertexBuffer->Unbind();
		s_DataUI.PlaneIndexBuffer->Unbind();
		s_DataUI.TextureShader->Unbind();


		VI_CORE_INFO("RendererAPI::GetDrawCallsCount = {}", RendererAPI::GetDrawCallsCount());
	}

	
	void RendererUI::DrawPlaneTwoSided(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();
		 
		DrawPlane(position, rotation, scale, color, tilingFactor);
		glm::vec3 Scale =  scale;
		Scale.z *= (-1);
		DrawPlane(position, rotation, Scale, color, tilingFactor);

	}


	void RendererUI::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

	//	s_DataUI.TextureShader->Bind();
		s_DataUI.TextureShader->SetFloat4("u_Color", color);

		constexpr glm::vec3 Positions[4] =
		{
			// face 1	
			{  -0.5f,  0.0f, -0.5f   },
			{   0.5f,  0.0f, -0.5f   },
			{   0.5f,  0.0f,  0.5f   },
			{  -0.5f,  0.0f,  0.5f   }
		}; 
		constexpr glm::vec2 textureCoords[4] =
		{
			{  0.0f, 0.0f  },
			{  1.0f, 0.0f  },
			{  1.0f, 1.0f  },
			{  0.0f, 1.0f  }
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		 
		glm::vec4 pos;

		for (size_t i = 0; i < s_DataUI.PlaneVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
		//	pos = glm::vec4{ Positions[i], 0.0f } / transform;
			//	s_DataUI.PlaneVertexBufferPtr->Position =  Offset + Positions[i] ; // working
			s_DataUI.PlaneVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
			s_DataUI.PlaneVertexBufferPtr->Color = color;
			s_DataUI.PlaneVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_DataUI.PlaneVertexBufferPtr->TexIndex = textureIndex;
			//	s_DataUI.PlaneVertexBufferPtr->TilingFactor = tilingFactor;
			s_DataUI.PlaneVertexBufferPtr++;
		}
		s_DataUI.PlaneIndexCount += s_DataUI.PlaneIndices;
	}
	
	void RendererUI::DrawCanvas(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

	//	s_DataUI.TextureShader->Bind();
		s_DataUI.TextureShader->SetFloat4("u_Color", color);
		
		

		constexpr glm::vec3 Positions[4] =
		{
			// face 1	
			{  -0.5f, -0.5f,  0.0f   },
			{  -0.5f,  0.5f,  0.0f   },
			{   0.5f,  0.5f,  0.0f   },
			{   0.5f, -0.5f,  0.0f   }
		}; 
		constexpr glm::vec2 textureCoords[4] =
		{
			{  0.0f, 0.0f  },
			{  1.0f, 0.0f  },
			{  1.0f, 1.0f  },
			{  0.0f, 1.0f  }
		};
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		
		glm::vec4 pos;

		for (size_t i = 0; i < s_DataUI.PlaneVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
			//pos = inverse * (glm::vec4(Positions[i], 1.0f) /*+ glm::vec4(position, 1.0f)*/ ) ;
			//	s_DataUI.PlaneVertexBufferPtr->Position =  Offset + Positions[i] ; // working
			s_DataUI.PlaneVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};

			VI_CORE_WARN("(RendererUI.cpp) : ColorAlpha = {}", color.a);
			s_DataUI.PlaneVertexBufferPtr->Color = color;// glm::vec4(color.r, color.g, color.b, 0.2f);
			s_DataUI.PlaneVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_DataUI.PlaneVertexBufferPtr->TexIndex = textureIndex;
			//	s_DataUI.PlaneVertexBufferPtr->TilingFactor = tilingFactor;
			s_DataUI.PlaneVertexBufferPtr++;
		}
		s_DataUI.PlaneIndexCount += s_DataUI.PlaneIndices;
	}


	void RendererUI::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_DataUI.TextureShader->SetFloat4("u_Color", tint);
		s_DataUI.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		s_DataUI.TextureShader->SetMat4("u_Transform", transform);


		s_DataUI.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_DataUI.PlaneVertexArray);

	}

}

