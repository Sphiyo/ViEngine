#include "Vipch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vi {



	Ref<Texture2D> Texture2D::Create(const std::string& path, uint32_t slot)
	{
		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:

			VI_CORE_ASSERT(false, "NoRendererAPI is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:

			return std::make_shared<OpenGLTexture2D>(path, slot);

		case RendererAPI::API::Vulcan:
			VI_CORE_ASSERT(false, "Vulcan is currently not supported!");
			return nullptr;

		case RendererAPI::API::D3D12:
			VI_CORE_ASSERT(false, "DX3D12 is currently not supported!");
			return nullptr;
		}

		VI_CORE_ASSERT(false, "Unknown Renderer API Selected !");
		return nullptr;
		return Ref<Texture2D>();
	}

}