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
				// Window Poll Event And Swap Buffers
				void OnUpdate() override;

				inline unsigned int GetWidth() const override { return m_Data.Width; }
				inline unsigned int GetHeight() const override { return m_Data.Height; }

				

					// Window attributes
				inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

				void SetVSync(bool enabled) override;
				bool ToggleVSync() override;
				bool IsVSync() const override;

				const CursorState& GetCursor() const override { return m_CursorState; }
				void SetCursor(CursorState cursorState, bool center = false) override;
				void ToggleCursor() override;
				void SetCursorPosition(double x, double y) override;

				// Ignoring monitorIndex Sets Index To The Last Active Monitor
				void SetFullScreen(bool enable, int16_t monitorIndex = 0) override;
				bool IsFullScreen() const override;
				bool& IsFullScreenRef() override;
				void ToggleFullScreen() override;

				void SetResolution(int width, int height) override;
				void SetWindowAspectRatio(int number, int denominator) override;
				void CheckMonitors() override;
			
		private:			
			virtual void Init(const WindowProps& props);
				virtual void Shutdown();
		private:
				GLFWwindow* m_Window;
				Scope<GraphicsContext> m_Context;
	
				struct WindowData {
					std::string Title;
					unsigned int Width, Height;
					bool VSync;
					bool FullScreen;
					EventCallbackFn EventCallback;
					std::vector<GLFWmonitor*>m_Monitors;
					int m_MonitorsCount =0;
					int m_ActiveMonitor =0;
					int m_AspectRatio[2] = { 16 ,9 };
				};

				WindowData m_Data;

				// Inherited via Window
				inline virtual void* GetNativeWindow() const override;

				CursorState m_CursorState;
				
		};

}