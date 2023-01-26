#include "Vipch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>


namespace Vi {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t slot) : m_Width(width), m_Height(height)  {

		m_Slot = slot;

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	

		/////////////// glCreateTextures//////////////////////////////////////

		glGenTextures(1, &m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		//////////////////////////////////////////////////////////////////////

		/////////////// glTextureStorage2D////////////////////////////////////
		glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);
		//////////////////////////////////////////////////////////////////////

		/////////////// glTextureParameteri //////////////////////////////////
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		
		//////////////////////////////////////////////////////////////////////

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, uint32_t slot) : m_Path(path) {

		m_Slot = slot;
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VI_CORE_ASSERT(data, "(OpenGLTexture) Failed To Upload Image! ");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		VI_CORE_ASSERT(internalFormat & dataFormat, "(OpenGLTexture) Texture Format Not Supported !");
		
		/////////////// glCreateTextures//////////////////////////////////////

		glGenTextures(1, &m_RendererID);		
		Bind(slot);
		

		
		//////////////////////////////////////////////////////////////////////


		/////////////// glTextureStorage2D////////////////////////////////////

		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, m_Width, m_Height);
		//	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		// 
		
		//////////////////////////////////////////////////////////////////////


		/////////////// glTextureParameteri //////////////////////////////////
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//////////////////////////////////////////////////////////////////////


	//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	
		/////////////// glTextureSubImage2D ////////////////////////////////////////////////////////

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		////////////////////////////////////////////////////////////////////////////////////////////

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {

		 // to do   // fix    // check    // test if the texture gets deleted from its slot or the 0 index one gets deleted
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size, uint32_t slot) {

		// byte per pixel
		uint32_t bpp = (m_DataFormat == GL_RGBA) ? (4) : (3);
		VI_CORE_ASSERT(size == m_Width * m_Height * bpp , "(OpenGLTexture) Data Must Be Entire Texture !");

		/////////////// glTextureSubImage2D ////////////////////////////////////////////////////////
		
		Bind(slot);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		////////////////////////////////////////////////////////////////////////////////////////////
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}


}