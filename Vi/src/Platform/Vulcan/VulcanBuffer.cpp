#include "Vipch.h"
#include "VulcanBuffer.h"



namespace Vi {

	///////////////////////////////////////////////////////////////////// VertexBuffer /////////////////////////////////////////////////////////////////////

	VulcanVertexBuffer::VulcanVertexBuffer(float* vertices, uint32_t size)
	{
	
	}

	VulcanVertexBuffer::~VulcanVertexBuffer()
	{

	}

	void VulcanVertexBuffer::Bind() const
	{

	}

	void VulcanVertexBuffer::Unbind() const
	{

	}



	void VulcanVertexBuffer::SetLayout(const BufferLayout& layout)
	{
	}


	///////////////////////////////////////////////////////////////////// IndexBuffer /////////////////////////////////////////////////////////////////////

	VulcanIndexBuffer::VulcanIndexBuffer(uint32_t* vertices, uint32_t count)
	{
	}

	VulcanIndexBuffer::~VulcanIndexBuffer()
	{
	}

	void VulcanIndexBuffer::Bind() const
	{
	}

	void VulcanIndexBuffer::Unbind() const
	{
	}

	uint32_t VulcanIndexBuffer::GetCount() const
	{
		return 0;
	}

}