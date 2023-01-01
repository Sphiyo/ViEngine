#include "Vipch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>

namespace Vi {


	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, uint32_t slot)
		: m_Path(path)
	{
	//	glEnable(GL_TEXTURE_2D);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VI_CORE_ASSERT(data, "Failed To Upload Image! ");
		m_Width = width;
		m_Height = height;
	//	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glGenTextures(1, &m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	//	glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_Width, m_Height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glTexSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
	//	VI_CORE_ERROR("~OpenGLTexture2D()");
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	//	glBindTextureUnit(slot, m_RendererID);
	}

}