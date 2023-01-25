#pragma once

#include "Vipch.h"
#include "Core.h"
#include "Vi/Events/Event.h"
#include "Vi/Renderer/GraphicsContext.h"

namespace Vi {

	enum class CursorState {

		NORMAL = 0, HIDDEN, DISABLED, UNAVAILABLE
	};

	struct WindowProps {

		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "VI Engine", uint32_t width = 1920, uint32_t height = 1080) {
			
			Title = title;
			Width = width;
			Height = height;
		}
	};

	// Interface representing a desktop system based Window
	class VI_API Window {
		
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual GraphicsContext* GetContext() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;
		static Window* Create(const WindowProps& props = WindowProps());

		virtual const CursorState& GetCursor() const = 0;
		virtual void SetCursor(CursorState cursorState, bool center = false) = 0;
		virtual void ToggleCursor() = 0;
		

		// Ignoring monitorIndex Sets Index To The Last Active Monitor
		virtual void SetFullScreen(bool enable, int16_t monitorIndex = -1) = 0;
		virtual bool IsFullScreen() const = 0;
		virtual bool& IsFullScreenRef() = 0;
		virtual void ToggleFullScreen() = 0;


		virtual void SetResolution(int width, int height) = 0;
		virtual void CheckMonitors() = 0;
	};

}