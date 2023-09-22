#include "Vipch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vi {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t slot) {


		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:
			VI_CORE_ASSERT(false, "(Texture) NoRendererAPI is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, slot);

		case RendererAPI::API::Vulcan:
			VI_CORE_ASSERT(false, "(Texture) Vulcan is currently not supported!");
			return nullptr;

		case RendererAPI::API::D3D12:
			VI_CORE_ASSERT(false, "(Texture) DX3D12 is currently not supported!");
			return nullptr;
		}

		VI_CORE_ASSERT(false, "(Texture) Unknown Renderer API Selected !");
		return nullptr;
	}
	



	Ref<Texture2D> Texture2D::Create(const std::string& path, uint32_t slot) {


		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VI_CORE_ASSERT(false, "(Texture) NoRendererAPI is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path, slot);

			case RendererAPI::API::Vulcan:
				VI_CORE_ASSERT(false, "(Texture) Vulcan is currently not supported!");
				return nullptr;

			case RendererAPI::API::D3D12:
				VI_CORE_ASSERT(false, "(Texture) DX3D12 is currently not supported!");
				return nullptr;
		}

		VI_CORE_ASSERT(false, "(Texture) Unknown Renderer API Selected !");
		return nullptr;
	}

}