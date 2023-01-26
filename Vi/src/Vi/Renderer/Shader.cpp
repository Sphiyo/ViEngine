#include "Vipch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Vi {

	Ref<Shader> Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:

			VI_CORE_ASSERT(false, "(Shader) NoRendererAPI is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:

			return CreateRef<OpenGLShader>(filepath);

		case RendererAPI::API::Vulcan:
			VI_CORE_ASSERT(false, "(Shader) Vulcan is currently not supported!");
			return nullptr;

		case RendererAPI::API::D3D12:
			VI_CORE_ASSERT(false, "(Shader) DX3D12 is currently not supported!");
			return nullptr;
		}

		VI_CORE_ASSERT(false, "(Shader) Unknown Renderer API Selected !");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const  std::string& fragmentSrc) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:

				VI_CORE_ASSERT(false, "(Shader) NoRendererAPI is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:

				return CreateRef<OpenGLShader>(name ,vertexSrc, fragmentSrc);

			case RendererAPI::API::Vulcan:
				VI_CORE_ASSERT(false, "(Shader) Vulcan is currently not supported!");
				return nullptr;

			case RendererAPI::API::D3D12:
				VI_CORE_ASSERT(false, "(Shader) DX3D12 is currently not supported!");
				return nullptr;
		}

		VI_CORE_ASSERT(false, "(Shader) Unknown Renderer API Selected !");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		VI_CORE_ASSERT(!Exists(name), "(Shader) This Shader Exists Already !");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		VI_CORE_ASSERT(Exists(name), "(Shader) Shader Not Found !");
		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}