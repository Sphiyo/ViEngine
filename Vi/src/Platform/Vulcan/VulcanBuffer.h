#pragma once

#include "Vi/Renderer/Buffer.h"


namespace Vi {


	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////

	class VulcanVertexBuffer : public VertexBuffer
	{
	public:
		VulcanVertexBuffer(float* vertices, uint32_t size);
		virtual ~VulcanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	
	private:

		//uint32_t m_RendererID;
	
	};


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////

	class VulcanIndexBuffer : public IndexBuffer
	{
	public:
		VulcanIndexBuffer(uint32_t* vertices, uint32_t count);
		virtual ~VulcanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		// Inherited via IndexBuffer
		virtual uint32_t GetCount() const override;
	};


}

