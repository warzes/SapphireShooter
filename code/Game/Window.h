#pragma once

#include "WindowDescription.h"
#include "NonCopyable.h"

class Window final : NonCopyable
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