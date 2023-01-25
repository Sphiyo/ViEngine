#include "Vipch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vi {

	Ref<VertexArray> VertexArray::Create(){

		switch (Renderer::GetAPI())	{

			case RendererAPI::API::None:
				VI_CORE_ASSERT(false, "(VertexArray) NoRendererAPI is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();

			case RendererAPI::API::Vulcan:
				VI_CORE_ASSERT(false, "(VertexArray) Vulcan is currently not supported!");			
				return nullptr;

			case RendererAPI::API::D3D12:
				VI_CORE_ASSERT(false, "(VertexArray) DX3D12 is currently not supported!");
				return nullptr;
		}
		VI_CORE_ASSERT(false, "(VertexArray) Unknown Renderer API Selected !");
		return nullptr;
	}

}