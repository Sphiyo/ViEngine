#include "Vipch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Vi {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	uint32_t RendererAPI::s_DrawCalls = 0;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		VI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}