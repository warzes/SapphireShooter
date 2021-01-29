#include "stdafx.h"
#include "Window.h"
#include "Engine.h"
#include "Log.h"
#include "Keyboard.h"
#include "Mouse.h"
//-----------------------------------------------------------------------------
static Window* thisWindow = nullptr;
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Engine& engine = Engine::Get();

	if (thisWindow && engine.IsRun())
	{
		Mouse::Get().HandleMsg(hwnd, msg, wparam, lparam);
		Keyboard::Get().HandleMsg(hwnd, msg, wparam, lparam);

		switch (msg)
		{
		case WM_ACTIVATE:
			switch (wparam)
			{
			default:
				break;

			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				Mouse::Get().Attach(hwnd);
				thisWindow->hasWindowFocus = true;
				break;

			case WA_INACTIVE:
				if (thisWindow->isFullScreen)
					ShowWindow(hwnd, SW_MINIMIZE);
				Mouse::Get().Detach();
				thisWindow->hasWindowFocus = false;
				break;
			}
			break;
		case WM_SIZE:
			// TODO: переделать
			glViewport(0, 0, LOWORD(lparam), HIWORD(lparam));
			engine.GetDescription().window.width = static_cast<int>(LOWORD(lparam));
			engine.GetDescription().window.height = static_cast<int>(HIWORD(lparam));
			break;

		case WM_SYSKEYDOWN:
			if (wparam == VK_RETURN)
				thisWindow->ToggleFullScreen();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}	
	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}
#endif
//-----------------------------------------------------------------------------
Window::~Window()
{
	if (m_isInit)
		Close();
}
//-----------------------------------------------------------------------------
bool Window::Init(const WindowDescription& config)
{
#if SE_PLATFORM_WINDOWS
	m_instance = GetModuleHandle(0);

	if (!registerClass())
		return false;	

	if (!createWindow(config))
		return false;

	Mouse::Get().Attach(m_hwnd);
#endif
	m_isInit = true;
	thisWindow = this;
	return true;
}
//-----------------------------------------------------------------------------
bool Window::Broadcast()
{
#if SE_PLATFORM_WINDOWS
	MSG msg;
	while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
	return true;
}
//-----------------------------------------------------------------------------
void Window::ToggleFullScreen()
{
#if SE_PLATFORM_WINDOWS
	static DWORD savedExStyle;
	static DWORD savedStyle;
	static RECT rcSaved;
	static auto& windowConfig = Engine::Get().GetDescription().window;

	isFullScreen = !isFullScreen;

	if (isFullScreen)
	{
		// Moving to full screen mode.

		savedExStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
		savedStyle = GetWindowLong(m_hwnd, GWL_STYLE);
		GetWindowRect(m_hwnd, &rcSaved);

		SetWindowLong(m_hwnd, GWL_EXSTYLE, 0);
		SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		windowConfig.width = GetSystemMetrics(SM_CXSCREEN);
		windowConfig.height = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, windowConfig.width, windowConfig.height, SWP_SHOWWINDOW);
	}
	else
	{
		// Moving back to windowed mode.

		SetWindowLong(m_hwnd, GWL_EXSTYLE, savedExStyle);
		SetWindowLong(m_hwnd, GWL_STYLE, savedStyle);
		SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		windowConfig.width = rcSaved.right - rcSaved.left;
		windowConfig.height = rcSaved.bottom - rcSaved.top;

		SetWindowPos(m_hwnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top, windowConfig.width, windowConfig.height, SWP_SHOWWINDOW);
	}
#endif
}
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
bool Window::registerClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = WS_EX_NOPARENTNOTIFY;
	wc.hbrBackground = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = m_instance;
	wc.lpszClassName = WINDOWCLASS;
	wc.lpszMenuName = 0;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;

	if (!::RegisterClassEx(&wc))
	{
		SE_ERROR("RegisterClassExW() failed: Can not register window class.");
		return false;
	}
	
	return true;
}
#endif
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
bool Window::createWindow(const WindowDescription& config)
{
	auto& windowConfig = Engine::Get().GetDescription().window;

	DWORD wndExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = config.width;
	windowRect.bottom = config.height;

	AdjustWindowRectEx(&windowRect, wndStyle, FALSE, wndExStyle);


	m_hwnd = ::CreateWindowEx(wndExStyle, WINDOWCLASS, L"Game", wndStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 0, 0, m_instance, 0);
	if (!m_hwnd)
	{
		SE_ERROR("CreateWindow() failed: Can not create window.");
		return false;
	}
	//if (!mDesc.fullscreen)
	{
		// Center on screen
		unsigned x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		unsigned y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(m_hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}




	//RECT rect = { 0 };
	//GetClientRect(m_hwnd, &rect);
	//windowConfig.width = rect.right - rect.left;
	//windowConfig.height = rect.bottom - rect.top;

	//MONITORINFO mi = { sizeof(mi) };
	//GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST), &mi);
	//int x = (mi.rcMonitor.right - mi.rcMonitor.left - windowConfig.width) / 2;
	//int y = (mi.rcMonitor.bottom - mi.rcMonitor.top - windowConfig.height) / 2;
	//SetWindowPos(m_hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);

	::ShowWindow(m_hwnd, SW_SHOW);
	::SetForegroundWindow(m_hwnd);
	::SetFocus(m_hwnd);
	::UpdateWindow(m_hwnd);
	return true;
}
#endif
//-----------------------------------------------------------------------------
void Window::Close()
{
#if SE_PLATFORM_WINDOWS
	DestroyWindow(m_hwnd);
	UnregisterClass(WINDOWCLASS, m_instance);
#endif
	m_isInit = false;
}
//-----------------------------------------------------------------------------