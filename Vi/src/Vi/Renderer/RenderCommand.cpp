#include "Vipch.h"
#include "RenderCommand.h"




namespace Vi {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}