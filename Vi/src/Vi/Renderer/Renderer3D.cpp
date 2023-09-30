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

	struct CubeVertex {

		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord; 
	//	float TexIndex;
	//	float TilingFactor;
	};

	struct Renderer3DData {
		const uint32_t CubeVertices = 4 * 6;
		const uint32_t CubeIndices = 6 * 6;

		const uint32_t MaxCubes = 150'000;
		const uint32_t MaxVertices = MaxCubes * CubeVertices;
		const uint32_t MaxIndices = MaxCubes * CubeIndices;
	//	static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
		
		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<VertexArray> PlaneVertexArray; 
		Ref<Shader> TextureShader;
		Ref<Shader> SppotlightShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

	//	std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
	//	uint32_t TextureSlotIndex = 1; // 0 = white texture
	};

	static Renderer3DData s_Data;


	void Renderer3D::Init() {
		VI_PROFILE_FUNCTION();

		s_Data.CubeVertexArray = VertexArray::Create();

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
////// Square VertexBuffer //////////////////////////////////////////////////////////////// batch ////////////////////////////////
//		float cubeVertices[ 5 * 4 * 6 ] = {																						//
//			// face 1																											//
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,																						//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,																						//
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,																						//
//		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,																						//
//																																//
//			// face 2																											//
//		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,																						//
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,																						//
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,																						//
//		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,																						//
//																																//
//			// face 3																											//
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,																						//
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,																						//
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,																						//
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,																						//
//																																//
//			// face 4																											//
//		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,																						//
//		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,																						//
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,																						//
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,																						//
//																																//
//			// face 5																											//
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,																						//
//		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,																						//
//		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,																						//
//		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,																						//
//																																//
//			// face 6																											//
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,																						//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,																						//
//		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,																						//
//		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,																						//
//																																//
//		};																														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
		

		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },    // TODO
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		//cubeVB->SetLayout({
		//	{ ShaderDataType::Float3, "a_Position" },
		//	{ ShaderDataType::Float2, "a_TexCoord" }
		//	});
		//s_Data.CubeVertexArray->AddVertexBuffer(cubeVB);

		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

		// to do make ref count for thread safety if the func got cued for longer time 
		uint32_t* cubeIndices = new uint32_t[s_Data.MaxIndices];
		 
		uint32_t Indices[6 * 6] = {

			0  ,  1  , 2   ,  2   , 3   , 0   ,
			4  ,  5  , 6   ,  6   , 7   , 4   ,
			8  ,  9  , 10  ,  10  , 11  , 8   ,
			12 , 13  , 14  ,  14  , 15  , 12  ,
			16 , 17  , 18  ,  18  , 19  , 16  ,
			20 , 21  , 22  ,  22  , 23  , 20

		};

		uint32_t offset = 0;

		// i = indices

		for ( uint32_t i = 0; i < s_Data.MaxIndices; i ++) {
			uint32_t DynamicIndices = i % s_Data.CubeIndices;
				cubeIndices[i ] = offset + Indices[DynamicIndices];
				if ( (DynamicIndices +1) == s_Data.CubeIndices )
					offset += s_Data.CubeVertices;
	
		}
	#if WORKING
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += (6 * 6) ) {
			for (uint32_t j = 0; j < 6 * 6; j++) {

				cubeIndices[i + j ] = offset + Indices[j];
				

			}
				offset += 4 * 6 ;
		}
	#endif
		// Cube IndexBuffer 
	//	uint32_t cubeIndices[ 6 * 6 ] = {
	//
	//		0  ,  1  , 2   ,  2   , 3   , 0   ,
	//		4  ,  5  , 6   ,  6   , 7   , 4   ,
	//		8  ,  9  , 10  ,  10  , 11  , 8   ,
	//		12 , 13  , 14  ,  14  , 15  , 12  ,
	//		16 , 17  , 18  ,  18  , 19  , 16  ,
	//		20 , 21  , 22  ,  22  , 23  , 20
	//
	//	};
		
		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_Data.MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(cubeIB);
		
		delete[] cubeIndices;
		// Good From Above ////////////////////////////////////////////////////////////////////////////////////// batch render

		/*s_Data.PlaneVertexArray = VertexArray::Create();

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
		s_Data.PlaneVertexArray->AddVertexBuffer(planeVB);

		// Plane IndexBuffer 
		uint32_t planeIndices[6] = { 0,1,2,2,3,0 };

		Ref<IndexBuffer> planeIB = IndexBuffer::Create(planeIndices, sizeof(planeIndices) / sizeof(uint32_t));
		s_Data.PlaneVertexArray->SetIndexBuffer(planeIB);*/

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create And Compile Shaders  
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
		s_Data->TextureShader->Unbind();
		s_Data->SppotlightShader = Shader::Create("assets/shaders/SpotLightShader.glsl");
		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetInt("u_Texture", 0);
		s_Data->SppotlightShader->Unbind();
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
		 
				
	}
	void Renderer3D::Shutdown() {
		VI_PROFILE_FUNCTION();


	}

	void Renderer3D::BeginScene(const Camera& camera) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix()); 		
		s_Data->TextureShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data->TextureShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data->TextureShader->Unbind();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		VI_CORE_INFO( "Cube Instance Count : {}", s_Data.CubeIndexCount / s_Data.CubeIndices );

		s_Data.CubeIndexCount = 0;
	
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->SppotlightShader->SetMat4("projection", camera.GetProjectionMatrix());
		s_Data->SppotlightShader->SetMat4("view", camera.GetViewMatrix());  // for spotlight
		s_Data->SppotlightShader->Unbind();
	}

	void Renderer3D::EndScene() {
		VI_PROFILE_FUNCTION();

		uint32_t datasize = (uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase;
		s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, datasize);

		Flush();
	}

	void Renderer3D::Flush() {
		//VI_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
	
		VI_CORE_INFO( "RendererAPI::GetDrawCallsCount = {}", RendererAPI::GetDrawCallsCount( ) );
	}
	void Renderer3D::DrawCube(const glm::vec3& camPosition, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->SppotlightShader->Bind();
		s_Data->SppotlightShader->SetFloat4("u_Color", color);
		s_Data->SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);

		s_Data->SppotlightShader->SetFloat3("cameraPos", camPosition);			// test
		s_Data->SppotlightShader->SetFloat("maxDistance", 10.0f);			// test

		s_Data.TextureShader->SetFloat4( "u_Color", color );

		constexpr glm::vec3 Positions[4 * 6] =
		{

			// face 1	
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{   0.5f, -0.5f, -0.5f   },

			// face 2			   
			{  -0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f, -0.5f   },


			// face 3			   
			{  -0.5f, -0.5f,  0.5f   },
			{  -0.5f,  0.5f,  0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },


			// face 4			   
			{   0.5f, -0.5f, -0.5f   },
			{   0.5f,  0.5f, -0.5f   },
			{   0.5f,  0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },


			// face 5			   
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f,  0.5f   },
			{   0.5f, -0.5f, -0.5f   },


			// face 6			   
			{  -0.5f, -0.5f, -0.5f   },
			{  -0.5f,  0.5f, -0.5f   },
			{  -0.5f,  0.5f,  0.5f   },
			{  -0.5f, -0.5f,  0.5f   }

		};
		/*	const glm::vec3 Positions[4 * 6] =
		{

			// face 1
			{  -scale.x, -scale.y, -scale.z   },
			{  -scale.x,  scale.y, -scale.z   },
			{   scale.x,  scale.y, -scale.z   },
			{   scale.x, -scale.y, -scale.z   },

			// face 2
			{  -scale.x,  scale.y, -scale.z   },
			{  -scale.x,  scale.y,  scale.z   },
			{   scale.x,  scale.y,  scale.z   },
			{   scale.x,  scale.y, -scale.z   },


			// face 3
			{  -scale.x, -scale.y,  scale.z   },
			{  -scale.x,  scale.y,  scale.z   },
			{   scale.x,  scale.y,  scale.z   },
			{   scale.x, -scale.y,  scale.z   },


			// face 4
			{   scale.x, -scale.y, -scale.z   },
			{   scale.x,  scale.y, -scale.z   },
			{   scale.x,  scale.y,  scale.z   },
			{   scale.x, -scale.y,  scale.z   },


			// face 5
			{  -scale.x, -scale.y, -scale.z   },
			{  -scale.x, -scale.y,  scale.z   },
			{   scale.x, -scale.y,  scale.z   },
			{   scale.x, -scale.y, -scale.z   },


			// face 6
			{  -scale.x, -scale.y, -scale.z   },
			{  -scale.x,  scale.y, -scale.z   },
			{  -scale.x,  scale.y,  scale.z   },
			{  -scale.x, -scale.y,  scale.z   }

		};
	*/
		/*constexpr glm::vec2 textureCoords[4] =
		{

			{  0.0f, 0.0f  },
			{  1.0f, 0.0f  },
			{  1.0f, 1.0f  },
			{  0.0f, 1.0f  }

		};*/
		const glm::vec2 textureCoords[4 * 6] =
		{

			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


			{  0.0f, 0.0f  },
			{  0.0f, 1.0f  },
			{  1.0f, 1.0f  },
			{  1.0f, 0.0f  },


		};

		const size_t cubeVertexCount = 4 * 6;

		glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), position ) *
			glm::rotate( glm::mat4( 1.0f ), glm::radians( rotation.x ), { 1.0f,0.0f,0.0f } ) *
			glm::rotate( glm::mat4( 1.0f ), glm::radians( rotation.y ), { 0.0f,1.0f,0.0f } ) *
			glm::rotate( glm::mat4( 1.0f ), glm::radians( rotation.z ), { 0.0f,0.0f,1.0f } ) *
			glm::scale( glm::mat4( 1.0f ), scale );


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
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

	//	s_Data.WhiteTexture->Bind();

	//	s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		
		
		

		for (size_t i = 0; i < cubeVertexCount; i++)  // test // return here // to do //experimental
		{
			glm::vec4 pos = transform * glm::vec4{ Positions[i],0.0f };
		for ( size_t i = 0; i < s_Data.CubeVertices; i++ ) {
			glm::vec4 pos = transform * glm::vec4{ Positions[i], 0.0f };
		//	s_Data.CubeVertexBufferPtr->Position =  position + Positions[i] ; // working
			s_Data.CubeVertexBufferPtr->Position = position + glm::vec3{pos.x, pos.y, pos.z};
			s_Data.CubeVertexBufferPtr->Color = color;
			s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
		//	s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
		//	s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr++;
		}
		s_Data.CubeIndexCount += s_Data.CubeIndices;

	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->SppotlightShader->SetFloat4("u_Color", color);
		s_Data->SppotlightShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data->SppotlightShader->SetMat4("u_Transform", transform);
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data->SppotlightShader->SetMat4("model", transform);

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
	}


	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		//s_Data->TextureShader->SetMat4("model", transform);

		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
		s_Data->TextureShader->Unbind();
		s_Data.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.PlaneVertexArray);
	}

	void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor) {
		VI_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetFloat4("u_Color", tint);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f,0.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f,1.0f,0.0f }) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f,0.0f,1.0f }) *
							  glm::scale(glm::mat4(1.0f), scale);

		s_Data.TextureShader->SetMat4("u_Transform", transform);


		s_Data->PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->PlaneVertexArray);
		s_Data->TextureShader->Unbind();
		s_Data.PlaneVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.PlaneVertexArray);
					
	}

}