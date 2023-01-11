#pragma once


namespace Vi {

	///////////////////////////////////////////////////////////////////// ShaderDataType /////////////////////////////////////////////////////////////////////


	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};


	/////////////////////////////////////////////////////////////////// ShaderDataTypeSize ///////////////////////////////////////////////////////////////////

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Vi::ShaderDataType::None:
			
		case Vi::ShaderDataType::Float    :    return 4 * 1 * 1;
									       
		case Vi::ShaderDataType::Float2   :    return 4 * 2 * 1;
									       
		case Vi::ShaderDataType::Float3   :    return 4 * 3 * 1;
									       
		case Vi::ShaderDataType::Float4   :    return 4 * 4 * 1;
									        
		case Vi::ShaderDataType::Mat3     :    return 4 * 3 * 3;
									       
		case Vi::ShaderDataType::Mat4     :    return 4 * 4 * 4;
									       
		case Vi::ShaderDataType::Int      :    return 4 * 1 * 1;
									       
		case Vi::ShaderDataType::Int2     :    return 4 * 2 * 1;
								  	       
		case Vi::ShaderDataType::Int3     :    return 4 * 3 * 1;
								  	       
		case Vi::ShaderDataType::Int4     :    return 4 * 4 * 1;
								  	       
		case Vi::ShaderDataType::Bool     :    return 1 * 1 * 1;
					
		}

		VI_CORE_ASSERT(false, "(Buffer) Unknown ShaderTypeData! ");
		return 0;
	}


	///////////////////////////////////////////////////////////////////// BufferElements /////////////////////////////////////////////////////////////////////

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{

				case Vi::ShaderDataType::Float   : return 1;
				
				case Vi::ShaderDataType::Float2  : return 2;
									     
				case Vi::ShaderDataType::Float3  : return 3;
									     
				case Vi::ShaderDataType::Float4  : return 4;
				
				case Vi::ShaderDataType::Mat3    : return 3 * 3;
									     
				case Vi::ShaderDataType::Mat4    : return 4 * 4;
									     
				case Vi::ShaderDataType::Int     : return 1;
									     
				case Vi::ShaderDataType::Int2    : return 2;
									     
				case Vi::ShaderDataType::Int3    : return 3;
									     
				case Vi::ShaderDataType::Int4    : return 4;
									     
				case Vi::ShaderDataType::Bool    : return 1;
			
			}

			VI_CORE_ASSERT(false, "(Buffer) Unknown ShaderTypeData! ");
			return 0;
		}

	};
	////////////////////////////////////////////////////////////////////// BufferLayout //////////////////////////////////////////////////////////////////////

	class BufferLayout
	{
	public:
		
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:

		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};



	////////////////////////////////////////////////////////////////////// VertexBuffer //////////////////////////////////////////////////////////////////////

	class VertexBuffer
	{
	public:

		virtual ~VertexBuffer() {}
		
		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;


		static VertexBuffer* Create(float* vertices , uint32_t size);
	};

	/////////////////////////////////////////////////////////////////////// IndexBuffer ///////////////////////////////////////////////////////////////////////


	class IndexBuffer
	{
	public:

		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};



}