#include "stdafx.h"
#include "Mouse.h"
#include "Window.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
Mouse& Mouse::Get() noexcept
{
	static Mouse theInstance;
	return theInstance;
}
//-----------------------------------------------------------------------------
void Mouse::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Skip emulated mouse events that are caused by touch
	bool emulatedMouse = (GetMessageExtraInfo() & 0xffffff00) == 0xff515700;

	switch (msg)
	{
	case WM_SIZE:
		// If mouse is currently hidden, update the clip region
		if (!m_mouseVisibleInternal)
			updateMouseClipping();
		break;

	case WM_MOUSEMOVE:
		if (!emulatedMouse)
		{
			Point2 newPosition;
			newPosition.x = (int)(short)LOWORD(lParam);
			newPosition.y = (int)(short)HIWORD(lParam);

			// Do not transmit mouse move when mouse should be hidden, but is not due to no input focus
			if (m_mouseVisibleInternal == m_mouseVisible)
			{
				Point2 delta = newPosition - m_mousePosition;
				onMouseMove(newPosition, delta);
				// Recenter in hidden mouse cursor mode to allow endless relative motion
				if (!m_mouseVisibleInternal && delta != Point2::Zero)
				{
					Point2 centr;
					centr.x = GetEngineDescription().window.width / 2;
					centr.y = GetEngineDescription().window.height / 2;
					SetMousePosition(centr);
				}
				else
					m_mousePosition = newPosition;
			}
			else
				m_mousePosition = newPosition;
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void Mouse::Update() noexcept
{
	m_mouseMove = Point2::Zero;

	for (int i = 0; i < MaxNumMouseButton; i++)
		m_pPrevButtonStates[i] = m_pCurrButtonStates[i];

	m_pCurrButtonStates[0] = (GetKeyState(VK_LBUTTON) & 0x8000) ? true : false;
	m_pCurrButtonStates[1] = (GetKeyState(VK_RBUTTON) & 0x8000) ? true : false;
	m_pCurrButtonStates[2] = (GetKeyState(VK_MBUTTON) & 0x8000) ? true : false;
}
//-----------------------------------------------------------------------------
void Mouse::SetMouseVisible(bool enable)
{
	if (enable != m_mouseVisible)
	{
		m_mouseVisible = enable;
		updateMouseVisible();
	}
}
//-----------------------------------------------------------------------------
void Mouse::SetMousePosition(const Point2& position)
{
	m_mousePosition = position;
	POINT screenPosition;
	screenPosition.x = position.x;
	screenPosition.y = position.y;
	ClientToScreen(Window::Get().GetHWND(), &screenPosition);
	SetCursorPos(screenPosition.x, screenPosition.y);
}

//-----------------------------------------------------------------------------
void Mouse::updateMouseVisible()
{
	// When the window is unfocused, mouse should never be hidden
	bool newMouseVisible = Window::Get().hasWindowFocus ? m_mouseVisible : true;
	if (newMouseVisible != m_mouseVisibleInternal)
	{
		ShowCursor(newMouseVisible ? TRUE : FALSE);
		m_mouseVisibleInternal = newMouseVisible;
	}

	updateMouseClipping();
}
//-----------------------------------------------------------------------------
void Mouse::updateMouseClipping()
{
	if (m_mouseVisibleInternal)
		ClipCursor(nullptr);
	else
	{
		const auto width = GetEngineDescription().window.width;
		const auto height = GetEngineDescription().window.height;

		POINT point = { 0, 0 };
		ClientToScreen(Window::Get().GetHWND(), &point);
		RECT mouseRect;
		mouseRect.left = point.x;
		mouseRect.top = point.y;
		mouseRect.right = point.x + width;
		mouseRect.bottom = point.y + height;
		ClipCursor(&mouseRect);
	}
}
//-----------------------------------------------------------------------------
void Mouse::onMouseMove(const Point2& position, const Point2& delta)
{
	m_mouseMove += delta;
	//mouseMoveEvent.position = position;
	//mouseMoveEvent.delta = delta;
	//SendEvent(mouseMoveEvent);
}
//-----------------------------------------------------------------------------