#pragma once

class Mouse
{
public:
	enum MouseButton
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT = 1,
		BUTTON_MIDDLE = 2
	};

	static Mouse& Get() noexcept;

	bool ButtonDown(MouseButton button) const noexcept
	{
		return m_pCurrButtonStates[button];
	}

	bool ButtonPressed(MouseButton button) const noexcept
	{
		return m_pCurrButtonStates[button] && !m_pPrevButtonStates[button];
	}

	bool ButtonUp(MouseButton button) const noexcept
	{
		return !m_pCurrButtonStates[button];
	}

	bool CursorIsVisible() const noexcept
	{
		return m_cursorVisible;
	}

	bool IsMouseSmoothing() const noexcept
	{
		return m_enableFiltering;
	}

	int xPosAbsolute() const noexcept
	{
		return m_xPosAbsolute;
	}

	int yPosAbsolute() const noexcept
	{
		return m_yPosAbsolute;
	}

	float xPosRelative() const noexcept
	{
		return m_xPosRelative;
	}

	float yPosRelative() const noexcept
	{
		return m_yPosRelative;
	}

	float WeightModifier() const noexcept
	{
		return m_weightModifier;
	}

	float WheelPos() const noexcept
	{
		return m_mouseWheel;
	}

	bool Attach(HWND hWnd) noexcept;
	void Detach() noexcept;
	void HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void HideCursor(bool hideCursor) noexcept;
	void SetPosition(UINT x, UINT y) noexcept;
	void SetWeightModifier(float weightModifier) noexcept;
	void SmoothMouse(bool smooth) noexcept;
	void Update() noexcept;

private:
	Mouse() noexcept;
	~Mouse();

	void performMouseFiltering(float x, float y) noexcept;
	void performMouseSmoothing(float x, float y) noexcept;

	static const float WEIGHT_MODIFIER;
	static const int HISTORY_BUFFER_SIZE = 10;
	static const int TEMP_BUFFER_SIZE = 40;

	static BYTE m_tempBuffer[TEMP_BUFFER_SIZE];

	HWND m_hWnd = nullptr;
	int m_xPosAbsolute = 0;
	int m_yPosAbsolute = 0;
	int m_historyBufferSize = 0;
	int m_mouseIndex = 0;
	int m_wheelDelta = 0;
	int m_prevWheelDelta = 0;
	float m_mouseWheel = 0.0f;
	float m_xPosRelative = 0;
	float m_yPosRelative = 0;
	float m_weightModifier = 0.0f;
	float m_filtered[2] = { 0 };
	float m_history[HISTORY_BUFFER_SIZE * 2] = { 0 };
	float m_mouseMovementX[2] = { 0.0f };
	float m_mouseMovementY[2] = { 0.0f };
	bool m_cursorVisible = true;
	bool m_enableFiltering = true;
	bool m_buttonStates[2][3] = { false };
	bool* m_pCurrButtonStates = nullptr;
	bool* m_pPrevButtonStates = nullptr;
};