#include "Vipch.h"
#include "D3D12Buffer.h"



namespace Vi {


	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////
	

	D3D12VertexBuffer::D3D12VertexBuffer(float* vertices, uint32_t size)
	{

	}

	D3D12VertexBuffer::~D3D12VertexBuffer()
	{

	}

	void D3D12VertexBuffer::Bind() const
	{

	}

	void D3D12VertexBuffer::Unbind() const
	{

	}



	void D3D12VertexBuffer::SetLayout(const BufferLayout& layout)
	{
	}


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////

	D3D12IndexBuffer::D3D12IndexBuffer(uint32_t* vertices, uint32_t count)
	{
	}

	D3D12IndexBuffer::~D3D12IndexBuffer()
	{
	}

	void D3D12IndexBuffer::Bind() const
	{
	}

	void D3D12IndexBuffer::Unbind() const
	{
	}

	uint32_t D3D12IndexBuffer::GetCount() const
	{
		return 0;
	}

}