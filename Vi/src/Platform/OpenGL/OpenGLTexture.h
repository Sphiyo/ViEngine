#pragma once

#include "Vi/Renderer/Texture.h"

#include <glad/glad.h>


namespace Vi {


	

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t slot);
		OpenGLTexture2D(const std::string& path, uint32_t slot);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth()  const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size, uint32_t slot) override;


		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width =0, m_Height =0;
		uint32_t m_RendererID = 0, m_Slot = 0;
		GLenum m_InternalFormat, m_DataFormat;

	};

}