#include "Vipch.h"
#include "Buffer.h"


#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Platform/Vulcan/VulcanBuffer.h"

#if VI_PLATFORM_WINDOWS

#include "Platform/D3D12/D3D12Buffer.h"

#endif 




namespace Vi {

	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////
	
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{

			case RendererAPI::None :

				VI_CORE_ASSERT(false, "NoRendererAPI is currently not supported!");
				return nullptr;

			case RendererAPI::OpenGL :
			
				return new OpenGLVertexBuffer(vertices , size);

			case RendererAPI::Vulcan :

				VI_CORE_ASSERT(false, "Vulcan is currently not supported!");
				return new VulcanVertexBuffer(vertices, size);
			
			case RendererAPI::D3D12 :

				VI_CORE_ASSERT(false, "DX3D12 is currently not supported!");
				return new D3D12VertexBuffer(vertices, size);
		
		}

		VI_CORE_ASSERT(false, "Unknown Renderer API Selected !");
		return nullptr;
	}


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////


	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::None :

			VI_CORE_ASSERT(false, "NoRendererAPI is currently not supported!");
			return nullptr;

		case RendererAPI::OpenGL :

			return new OpenGLIndexBuffer(indices, count);

		case RendererAPI::Vulcan :

			VI_CORE_ASSERT(false, "Vulcan is currently not supported!");
			return new VulcanIndexBuffer(indices, count);

		case RendererAPI::D3D12 :

			VI_CORE_ASSERT(false, "DX3D12 is currently not supported!");
			return new D3D12IndexBuffer(indices, count);

		}

		VI_CORE_ASSERT(false, "Unknown Renderer API Selected !");
		return nullptr;
	}

}