#include "Vipch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Vi {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;



}