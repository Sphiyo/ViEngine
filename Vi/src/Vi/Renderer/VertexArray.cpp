#include "Vipch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vi {

	VertexArray* VertexArray::Create(){

		switch (Renderer::GetAPI())	{

			case RendererAPI::API::None:
				VI_CORE_ASSERT(false, "NoRendererAPI is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();

			case RendererAPI::API::Vulcan:
				VI_CORE_ASSERT(false, "Vulcan is currently not supported!");			
				return nullptr;

			case RendererAPI::API::D3D12:
				VI_CORE_ASSERT(false, "DX3D12 is currently not supported!");
				return nullptr;
		}
		VI_CORE_ASSERT(false, "Unknown Renderer API Selected !");
		return nullptr;
	}

}