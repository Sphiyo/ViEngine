#pragma once

#include "Vi/Renderer/Buffer.h"


namespace Vi {

	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////


	class D3D12VertexBuffer : public VertexBuffer
	{

	public:
		D3D12VertexBuffer(float* vertices, uint32_t size);
		virtual ~D3D12VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	
	private:

		//uint32_t m_RendererID;
	
	};


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////

	class D3D12IndexBuffer : public IndexBuffer
	{

	public:
		D3D12IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~D3D12IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:

		//uint32_t m_RendererID;

		// Inherited via IndexBuffer
		virtual uint32_t GetCount() const override;
	};


}

