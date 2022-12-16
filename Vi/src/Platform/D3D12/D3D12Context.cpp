#include "Vipch.h"
#include "D3D12Context.h"




namespace Vi {



	D3D12Context::D3D12Context(Win32Api* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VI_CORE_ASSERT(windowHandle, "Window handle is NULL");
	}

	void D3D12Context::Init()
	{
		
	}

	void D3D12Context::SwapBuffers()
	{
	
		
	}

}