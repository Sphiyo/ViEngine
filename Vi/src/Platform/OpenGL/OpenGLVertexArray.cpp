#include "Vipch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Vi {


	static GLenum ShaderDataToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{

			case Vi::ShaderDataType::Float:   return GL_FLOAT;

			case Vi::ShaderDataType::Float2:   return GL_FLOAT;

			case Vi::ShaderDataType::Float3:   return GL_FLOAT;

			case Vi::ShaderDataType::Float4:   return GL_FLOAT;

			case Vi::ShaderDataType::Mat3:   return GL_FLOAT;

			case Vi::ShaderDataType::Mat4:   return GL_FLOAT;

			case Vi::ShaderDataType::Int:   return GL_INT;

			case Vi::ShaderDataType::Int2:   return GL_INT;

			case Vi::ShaderDataType::Int3:   return GL_INT;

			case Vi::ShaderDataType::Int4:   return GL_INT;

			case Vi::ShaderDataType::Bool:   return GL_BOOL;

		}
		VI_CORE_ASSERT(false, "(OpenGLVertexArray) Unknown ShaderTypeData! ");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		//glCreateVertexArrays(1, &m_RendererID);
		glGenVertexArrays(1, &m_RendererID);
	}


	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}


	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}


	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}


	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		VI_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "(OpenGLVertexArray) Vertex Buffer has no layout!");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataToOpenGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;

	}



}