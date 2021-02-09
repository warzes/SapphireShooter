#include "stdafx.h"
#include "GameApp.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
EngineDescription GameApp::InitConfig()
{
	EngineDescription config;

	config.window.width = 1440;
	config.window.height = 900;
	return config;
}
//-----------------------------------------------------------------------------
GameApp::GameApp()
{
}
//-----------------------------------------------------------------------------
GameApp::~GameApp()
{
}
//-----------------------------------------------------------------------------
void GameApp::Init()
{

}
//-----------------------------------------------------------------------------
void GameApp::Update(float dt)
{
	resizeApp();
}
//-----------------------------------------------------------------------------
void GameApp::ProcessInput(float dt)
{
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
	m_framebuffer.ActivateFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// TODO: render to texture
	m_framebuffer.DeactivateFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void GameApp::Close()
{
	m_framebuffer.DestroyFramebuffer();
}
//-----------------------------------------------------------------------------
void GameApp::resizeApp()
{
	if (m_width != GetEngineDescription().window.width || m_height != GetEngineDescription().window.height)
	{
		m_width = GetEngineDescription().window.width;
		m_height = GetEngineDescription().window.height;
		
		const float aspectRatio = float(m_width) / float(m_height);
		m_mainCamera.InitCameraPerspective(80.0f, aspectRatio, 0.1f, 5000.0f);
		
		m_framebuffer.DestroyFramebuffer();
		m_framebuffer.CreateFramebuffer(m_width, m_height);
	}
}
//-----------------------------------------------------------------------------