#pragma once

#include "Vi/Core/Window.h"
#include "Vi/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Vi {

		class WindowsWindow : public Window
		{	
		public:
				WindowsWindow(const WindowProps& props);
				virtual ~WindowsWindow();

				void OnUpdate() override;

				inline unsigned int GetWidth() const override { return m_Data.Width; }
				inline unsigned int GetHeight() const override { return m_Data.Height; }

					// Window attributes
				inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

				void SetVSync(bool enabled) override;
				bool IsVSync() const override;

				void ShowCursor(bool show) const override;
				void SetFullScreen(bool enable) override;
				bool IsFullScreen() const override;
				bool& IsFullScreenRef() override;
				void SetResolution(int width, int height) override;
			
		private:			
			virtual void Init(const WindowProps& props);
				virtual void Shutdown();
		private:
				GLFWwindow* m_Window;

				GraphicsContext* m_Context;
	
				struct WindowData
				{
						std::string Title;
						unsigned int Width, Height;
						bool VSync;
						bool FullScreen;
						EventCallbackFn EventCallback;						
						std::vector<GLFWmonitor*>m_Monitors;						
						int m_ActiveMonitor;
				};

				WindowData m_Data;

				// Inherited via Window
				inline virtual void* GetNativeWindow() const override;

				
		};

}