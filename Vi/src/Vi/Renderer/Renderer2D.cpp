#include "Vipch.h"
#include "Renderer2D.h"

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

	struct Renderer2DData {
		const uint32_t PlaneVertices = 4;
		const uint32_t PlaneIndices = 6;

		const uint32_t MaxPlane = 100'000;
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

	static Renderer2DData s_Data2D;


	void Renderer2D::Init() {

		VI_PROFILE_FUNCTION();

		s_Data2D.PlaneVertexArray = VertexArray::Create();


		s_Data2D.PlaneVertexBuffer = VertexBuffer::Create(s_Data2D.MaxVertices * sizeof(PlaneVertex));


		s_Data2D.PlaneVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },    // TODO
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data2D.PlaneVertexArray->AddVertexBuffer(s_Data2D.PlaneVertexBuffer);


		s_Data2D.PlaneVertexBufferBase = new PlaneVertex[s_Data2D.MaxVertices];

		// to do make ref count for thread safety if the func got cued for longer time 
		
		uint32_t* planeIndices = new uint32_t[s_Data2D.MaxIndices];

		uint32_t Indices[6] = { 0, 1, 2, 2, 3, 0 };

		uint32_t offset = 0;

		// i = indices
		for (uint32_t i = 0; i < s_Data2D.MaxIndices; i++) {
			uint32_t DynamicIndices = i % s_Data2D.PlaneIndices;
			planeIndices[i] = offset + Indices[DynamicIndices];
			if ((DynamicIndices + 1) == s_Data2D.PlaneIndices)
				offset += s_Data2D.PlaneVertices;
		}
		
		s_Data2D.PlaneIndexBuffer = IndexBuffer::Create(planeIndices,s_Data2D.MaxIndices);
	//	Ref<IndexBuffer> cubeIB = IndexBuffer::Create(planeIndices, s_Data2D.MaxIndices);
		s_Data2D.PlaneVertexArray->SetIndexBuffer(s_Data2D.PlaneIndexBuffer);

		delete[] planeIndices;
		

		s_Data2D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data2D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_Data2D.TextureShader = Shader::Create("assets/shaders/UI.glsl");

		s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetInt("u_Texture", 0);

		s_Data2D.TextureShader->Unbind();
		s_Data2D.PlaneVertexArray->Unbind();
		s_Data2D.PlaneVertexBuffer->Unbind();
		s_Data2D.PlaneIndexBuffer->Unbind();

	}
	void Renderer2D::Shutdown() {
		VI_PROFILE_FUNCTION();


	}

	void Renderer2D::BeginScene(const Camera2D& camera) {
		VI_PROFILE_FUNCTION();


		s_Data2D.PlaneVertexArray->Bind();
		s_Data2D.PlaneVertexBuffer->Bind();
		s_Data2D.PlaneIndexBuffer->Bind();
		s_Data2D.TextureShader->Bind();
	//	VI_CORE_WARN("(Renderer2D.cpp) : ViewProj = {}", camera.GetViewProjectionMatrix());
		
	//	s_Data2D.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		

		int PlaneInstanceCount = s_Data2D.PlaneIndexCount / s_Data2D.PlaneIndices;
		 
		VI_CORE_INFO("Plane Instance Count : {}", PlaneInstanceCount); 

		VI_CORE_INFO("Total Indices Count : {}", (s_Data2D.PlaneIndexCount));
		//	VI_CORE_INFO( "Total Vertex Count : {}", ( s_Data2D.PlaneIndexCount / s_Data2D.PlaneIndices ));
		//	s_Data2D.PlaneIndexCount %= s_Data2D.CubeIndices; 
		s_Data2D.PlaneIndexCount = 0;

		s_Data2D.PlaneVertexBufferPtr = s_Data2D.PlaneVertexBufferBase;

	}


	void Renderer2D::EndScene() {
		VI_PROFILE_FUNCTION();
	//	s_Data2D.PlaneVertexArray->Bind();
	//	s_Data2D.PlaneVertexBuffer->Bind();
	//	s_Data2D.PlaneIndexBuffer->Bind();

		uint32_t datasize = (uint32_t)((uint8_t*)s_Data2D.PlaneVertexBufferPtr - (uint8_t*)s_Data2D.PlaneVertexBufferBase);
		s_Data2D.PlaneVertexBuffer->SetData(s_Data2D.PlaneVertexBufferBase, datasize);


		Flush();
	}

	void Renderer2D::Flush() {
		//VI_PROFILE_FUNCTION();
		s_Data2D.TextureShader->Bind();
		RenderCommand::DrawIndexed(s_Data2D.PlaneVertexArray, s_Data2D.PlaneIndexCount);

		s_Data2D.PlaneVertexArray->Unbind();
		s_Data2D.PlaneVertexBuffer->Unbind();
		s_Data2D.PlaneIndexBuffer->Unbind();
		s_Data2D.TextureShader->Unbind();


		VI_CORE_INFO("RendererAPI::GetDrawCallsCount = {}", RendererAPI::GetDrawCallsCount());
	}

	
	void Renderer2D::DrawPlaneTwoSided(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();
		 
		DrawPlane(position, rotation, scale, color, tilingFactor);
		glm::vec3 Scale =  scale;
		Scale.z *= (-1);
		DrawPlane(position, rotation, Scale, color, tilingFactor);

	}


	void Renderer2D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

	//	s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetFloat4("u_Color", color);

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

		glm::mat4 transformOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) *
			glm::rotate(glm::mat4(1.0f), 0.0f, { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		glm::vec4 pos;

		for (size_t i = 0; i < s_Data2D.PlaneVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
		//	pos = glm::vec4{ Positions[i], 0.0f } / transform;
			//	s_Data2D.PlaneVertexBufferPtr->Position =  Offset + Positions[i] ; // working
			s_Data2D.PlaneVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
			s_Data2D.PlaneVertexBufferPtr->Color = color;
			s_Data2D.PlaneVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_Data2D.PlaneVertexBufferPtr->TexIndex = textureIndex;
			//	s_Data2D.PlaneVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data2D.PlaneVertexBufferPtr++;
		}
		s_Data2D.PlaneIndexCount += s_Data2D.PlaneIndices;
	}
	
	void Renderer2D::DrawCanvas(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

	//	s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetFloat4("u_Color", color);
		glm::vec3 Scale = scale;
		//AspctRatio
		float AR = CameraController2D::s_CameraController2D->GetCamera().GetCameraProperties().AspectRatio;
		Scale.x /= AR;
		Scale.z /= AR;
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
			glm::scale(glm::mat4(1.0f), Scale);

		
		glm::vec4 pos;

		for (size_t i = 0; i < s_Data2D.PlaneVertices; i++) {
			pos = transform * glm::vec4{ Positions[i], 0.0f };
			//pos = inverse * (glm::vec4(Positions[i], 1.0f) /*+ glm::vec4(position, 1.0f)*/ ) ;
			//	s_Data2D.PlaneVertexBufferPtr->Position =  Offset + Positions[i] ; // working
			s_Data2D.PlaneVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};

			VI_CORE_WARN("(Renderer2D.cpp) : ColorAlpha = {}", color.a);
			s_Data2D.PlaneVertexBufferPtr->Color = color;// glm::vec4(color.r, color.g, color.b, 0.2f);
			s_Data2D.PlaneVertexBufferPtr->TexCoord = textureCoords[i];
			//	s_Data2D.PlaneVertexBufferPtr->TexIndex = textureIndex;
			//	s_Data2D.PlaneVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data2D.PlaneVertexBufferPtr++;
		}
		s_Data2D.PlaneIndexCount += s_Data2D.PlaneIndices;
	}


	void Renderer2D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data2D.TextureShader->SetFloat4("u_Color", tint);
		s_Data2D.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
			glm::scale(glm::mat4(1.0f), scale);

		s_Data2D.TextureShader->SetMat4("u_Transform", transform);


		s_Data2D.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D.PlaneVertexArray);

	}

}

