#include "Vipch.h"
#include "Renderer.h"

#include "Renderer3D.h"
#include "RendererUI.h" 



namespace Vi {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Scope<Renderer::SceneData> Renderer::s_SceneData2D = CreateScope<Renderer::SceneData>();


	void Renderer::Init() {
		VI_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer3D::Init();
		RendererUI::Init();

	}
	
	void Renderer::Shutdown()
	{
		Renderer3D::Shutdown();
		RendererUI::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0,width,height);
	}

	/*void Renderer::BeginScene(Camera& camera) {
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix3D();
		s_SceneData2D->ViewProjectionMatrix = camera.GetViewProjectionMatrix2D();
	}*/
	void Renderer::EndScene() {
	}

	/*
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,const glm::mat4& transform) {
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_ViewProjection", s_SceneData2D->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		//mi.Bind();		//BindShaderAndUpdloadAllUniform										 // todo
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	*/
}

