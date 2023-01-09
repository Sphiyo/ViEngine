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

			//	void window_size_callback(GLFWwindow* window, int width, int height);

				void OnUpdate() override;

				inline unsigned int GetWidth() const override { return m_Data.Width; }
				inline unsigned int GetHeight() const override { return m_Data.Height; }

					// Window attributes
				inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
				void SetVSync(bool enabled) override;
				bool IsVSync() const override;

				void ShowCursor(bool show) override;

			//	void (*funcs)(GLFWwindow* window, int button, int action, int mods){};
			//void function(GLFWwindow* window, int button, int action, int mods);
			//void (*GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);
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

						EventCallbackFn EventCallback;
				};

				WindowData m_Data;

				// Inherited via Window
				inline virtual void* GetNativeWindow() const override;

				
		};

}