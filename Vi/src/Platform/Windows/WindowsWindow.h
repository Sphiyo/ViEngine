#pragma once

#include "Vi/Core/Window.h"


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

				GraphicsContext* GetContext() const override { return m_Context; }

					// Window attributes
				inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

				void SetVSync(bool enabled) override;
				bool IsVSync() const override;

				const CursorState& GetCursor() const override { return m_CursorState; }
				void SetCursor(CursorState cursorState, bool center = false) override;

				void ToggleCursor() override;

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
	
				struct WindowData {
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

				CursorState m_CursorState;
				
		};

}