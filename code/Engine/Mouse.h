#pragma once

#include "Point2.h"
#include "NonCopyable.h"

enum class MouseButton : uint8_t
{
	Left,
	Right,
	Middle
};
constexpr uint8_t MaxNumMouseButton = 3;

class Mouse : NonCopyable
{
public:
	static Mouse& Get() noexcept;

	void HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void Update() noexcept;

	// Return accumulated mouse movement since last frame.
	Point2 GetMouseMove() const { return m_mouseMove; }
	// Return absolute mouse coordinate in window
	Point2 GetMousePosition() const { return m_mousePosition; }

	bool ButtonDown(MouseButton button) const noexcept
	{
		return m_pCurrButtonStates[static_cast<uint8_t>(button)];
	}

	bool ButtonPressed(MouseButton button) const noexcept
	{
		return m_pCurrButtonStates[static_cast<uint8_t>(button)] && !m_pPrevButtonStates[static_cast<uint8_t>(button)];
	}

	bool ButtonUp(MouseButton button) const noexcept
	{
		return !m_pCurrButtonStates[static_cast<uint8_t>(button)];
	}

	// Set mouse cursor visible. Default is true. When hidden, the mouse cursor is confined to the window and kept centered; relative mouse motion can be read "endlessly" but absolute mouse position should not be used.
	void SetMouseVisible(bool enable);
	// Move the mouse cursor to a window top-left relative position.
	void SetMousePosition(const Point2& position);
	
private:
	Mouse() = default;

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

	bool m_pCurrButtonStates[MaxNumMouseButton] = { false };
	bool m_pPrevButtonStates[MaxNumMouseButton] = { false };
};