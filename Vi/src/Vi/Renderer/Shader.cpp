#include "Vipch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Vi {

	Shader* Shader::Create(const std::string& vertexSrc, const  std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:

			VI_CORE_ASSERT(false, "NoRendererAPI is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:

			return new OpenGLShader(vertexSrc, fragmentSrc);

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