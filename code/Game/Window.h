#pragma once

#include "WindowDescription.h"

class Window final
{
public:
	Window() = default;
	~Window();

	bool Init(const WindowDescription& config);
	bool Broadcast();
	void Close();

	void ToggleFullScreen();

#if SE_PLATFORM_WINDOWS
	HWND GetHWND() const { return m_hwnd; }
#endif

	static Window& Get();

	bool hasWindowFocus = true; // TODO:

private:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
	
#if SE_PLATFORM_WINDOWS
	bool initDPI();
	bool registerClass();
	bool createWindow(const WindowDescription& config);
#endif
	
#if SE_PLATFORM_WINDOWS
	HWND m_hwnd = nullptr;
	HINSTANCE m_instance = nullptr;
#endif
	bool m_isInit = false;
};