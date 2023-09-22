#include "Vipch.h"
#include "Buffer.h"


#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

//#include "Platform/Vulcan/VulcanBuffer.h"

//#if VI_PLATFORM_WINDOWS
//
//#include "Platform/D3D12/D3D12Buffer.h"
//
//#endif 




namespace Vi {

	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////
	
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{

			case RendererAPI::API::None :

				VI_CORE_ASSERT(false, "(Buffer) RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL :
			
				return CreateRef<OpenGLVertexBuffer>(vertices, size);

			case RendererAPI::API::Vulcan :

				VI_CORE_ASSERT(false, "(Buffer) Vulcan is currently not supported!");
		//		return new VulcanVertexBuffer(vertices, size);
				return nullptr;

			case RendererAPI::API::D3D12 :

				VI_CORE_ASSERT(false, "(Buffer) DX3D12 is currently not supported!");
		//		return new D3D12VertexBuffer(vertices, size);
				return nullptr;

		}

		VI_CORE_ASSERT(false, "(Buffer) Unknown Renderer API Selected !");
		return nullptr;
	}


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None :

			VI_CORE_ASSERT(false, "(Buffer) RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL :

			return CreateRef<OpenGLIndexBuffer>(indices, count);

		case RendererAPI::API::Vulcan :

			VI_CORE_ASSERT(false, "(Buffer) Vulcan is currently not supported!");
		//	return new VulcanIndexBuffer(indices, count);
			return nullptr;

		case RendererAPI::API::D3D12 :

			VI_CORE_ASSERT(false, "(Buffer) DX3D12 is currently not supported!");
		//	return new D3D12IndexBuffer(indices, count);
			return nullptr;
		}

		VI_CORE_ASSERT(false, "(Buffer) Unknown Renderer API Selected !");
		return nullptr;
	}

}