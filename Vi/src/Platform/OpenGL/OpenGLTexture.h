#pragma once

#include "Vi/Renderer/Texture.h"

namespace Vi {


	

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, uint32_t slot);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth()  const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual void Bind(uint32_t slot) const override;
	private:
		std::string m_Path;
		uint32_t m_Width =0, m_Height =0;
		uint32_t m_RendererID = 0;

	};

}