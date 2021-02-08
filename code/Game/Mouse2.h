#pragma once

#include "Point2.h"

class Mouse2
{
public:
public:
	static Mouse2& Get() noexcept;

	// Set mouse cursor visible. Default is true. When hidden, the mouse cursor is confined to the window and kept centered; relative mouse motion can be read "endlessly" but absolute mouse position should not be used.
	void SetMouseVisible(bool enable);
	// Move the mouse cursor to a window top-left relative position.
	void SetMousePosition(const Point2& position);

	void Update() noexcept;
	void HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	// Return accumulated mouse movement since last frame.
	Point2 MouseMove() const { return m_mouseMove; }

private:
	// Update mouse visibility and clipping region to the OS.
	void updateMouseVisible();
	// Update mouse clipping region.
	void updateMouseClipping();
	// React to a mouse move. Called by window message handling.
	void onMouseMove(const Point2& position, const Point2& delta);

	// Current mouse cursor position.
	Point2 m_mousePosition;
	// Mouse visible flag as requested by the application.
	bool m_mouseVisible = true;
	//Internal mouse visible flag.The mouse is automatically shown when the window is unfocused, while mouseVisible represents the application's desired state. Used to prevent multiple calls to OS mouse visibility functions, which utilize a counter.
	bool m_mouseVisibleInternal = true;
	// Accumulated mouse move since last frame.
	Point2 m_mouseMove;
};