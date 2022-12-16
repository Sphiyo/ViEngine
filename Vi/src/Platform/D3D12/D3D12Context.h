#pragma once

#include "Vi/Renderer/GraphicsContext.h"

struct Win32Api;

namespace Vi {

	class D3D12Context : public GraphicsContext
	{
	public:
		D3D12Context(Win32Api* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		Win32Api* m_WindowHandle;
		float x = 0.0f;
		float y = 1.0f;
		bool Addx = true;
		bool Addy = true;
	};

}