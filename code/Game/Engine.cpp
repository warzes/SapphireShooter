#include "stdafx.h"
#include "Engine.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Mouse2.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ShaderManager.h"
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineDescription& desc)
{
	m_desc = desc;

#if SE_DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

	m_isRun = true;

	if (!m_window.Init(m_desc.window))
		return false;

#if SE_PLATFORM_WINDOWS
	if (!m_renderSystem.Init(m_window.GetHWND(), m_desc.render))
		return false;
#endif

	m_isEnd = false;

	return true;
}
//-----------------------------------------------------------------------------
void Engine::BeginUpdate()
{
	if (m_isEnd) return;

	Mouse2::Get().Update();
	m_isEnd = !m_window.Broadcast();
	Mouse::Get().Update();
	Keyboard::Get().Update();
}
//-----------------------------------------------------------------------------
void Engine::EndUpdate()
{
	if (m_isEnd) return;
}
//-----------------------------------------------------------------------------
void Engine::BeginFrame()
{
	if (m_isEnd) return;

	if (m_window.hasWindowFocus)
		m_renderSystem.BeginFrame(m_desc.window.width, m_desc.window.height);
}
//-----------------------------------------------------------------------------
void Engine::EndFrame()
{
	if (m_isEnd) return;

	if (m_window.hasWindowFocus)
	{
		m_renderSystem.EndFrame();
	}
	else
		::WaitMessage();
}
//-----------------------------------------------------------------------------
void Engine::Close() noexcept
{
	Mouse2::Get().SetMouseVisible(true);
	TextureManager::Get().Clear();
	ShaderManager::Get().Clear();
	m_renderSystem.Close();
	m_window.Close();
	m_isRun = false;
}
//-----------------------------------------------------------------------------