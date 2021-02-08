#include "stdafx.h"
#include "Mouse2.h"
#include "Window.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
Mouse2& Mouse2::Get() noexcept
{
	static Mouse2 theInstance;
	return theInstance;
}
//-----------------------------------------------------------------------------
void Mouse2::SetMouseVisible(bool enable)
{
	if (enable != m_mouseVisible)
	{
		m_mouseVisible = enable;
		updateMouseVisible();
	}
}
//-----------------------------------------------------------------------------
void Mouse2::SetMousePosition(const Point2& position)
{
	m_mousePosition = position;
	POINT screenPosition;
	screenPosition.x = position.x;
	screenPosition.y = position.y;
	ClientToScreen(Window::Get().GetHWND(), &screenPosition);
	SetCursorPos(screenPosition.x, screenPosition.y);
}
//-----------------------------------------------------------------------------
void Mouse2::Update() noexcept
{
	m_mouseMove = Point2::Zero;
}
//-----------------------------------------------------------------------------
void Mouse2::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Skip emulated mouse events that are caused by touch
	bool emulatedMouse = (GetMessageExtraInfo() & 0xffffff00) == 0xff515700;

	switch (msg)
	{
	//case WM_ACTIVATE:
	//	updateMouseVisible();
	//	break;

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
void Mouse2::updateMouseVisible()
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
void Mouse2::updateMouseClipping()
{
	if (m_mouseVisibleInternal)
		ClipCursor(nullptr);
	else
	{
		auto width = GetEngineDescription().window.width;
		auto height = GetEngineDescription().window.height;

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
void Mouse2::onMouseMove(const Point2& position, const Point2& delta)
{
	m_mouseMove += delta;
	//mouseMoveEvent.position = position;
	//mouseMoveEvent.delta = delta;
	//SendEvent(mouseMoveEvent);
}
//-----------------------------------------------------------------------------