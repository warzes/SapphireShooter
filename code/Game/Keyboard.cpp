#include "stdafx.h"
#include "Keyboard.h"
//-----------------------------------------------------------------------------
// TODO: переделать HandleMsg и Update в одну функцию
//-----------------------------------------------------------------------------
Keyboard& Keyboard::Get() noexcept
{
	static Keyboard theInstance;
	return theInstance;
}
//-----------------------------------------------------------------------------
Keyboard::Keyboard() noexcept
{
	m_pCurrKeyStates = m_keyStates[0];
	m_pPrevKeyStates = m_keyStates[1];
	memset(m_pCurrKeyStates, 0, 256);
	memset(m_pPrevKeyStates, 0, 256);
}
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
void Keyboard::HandleMsg(HWND /*hWnd*/, UINT msg, WPARAM wParam, LPARAM /*lParam*/) noexcept
{
	// TODO: как-то убрать эту функцию
	switch (msg)
	{
	case WM_CHAR:
		m_lastChar = static_cast<int>(wParam);
		break;
	default:
		break;
	}
}
#endif
//-----------------------------------------------------------------------------
void Keyboard::Update() noexcept
{
#if SE_PLATFORM_WINDOWS
	// TODO: в будущем найти лучшее решение, а не копирование всех состояний клавиш
	BYTE* pTempKeyStates = m_pPrevKeyStates;

	m_pPrevKeyStates = m_pCurrKeyStates;
	m_pCurrKeyStates = pTempKeyStates;

	GetKeyboardState(m_pCurrKeyStates);
#endif
}
//-----------------------------------------------------------------------------