#include "stdafx.h"
#include "GameApp.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse2.h"
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
	Player::Get().Init(m_mainCamera, glm::vec3(256.0f, 0.0f, 300.0f));

	m_dirLight.Configure(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	m_dirLight.SetDirection(glm::vec3(0.2f, 1.0f, 0.5f));
	m_dirLight.SetColour(glm::vec3(0.97f, 0.88f, 0.70f));

	m_pointLight.Configure(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045f, 0.0075f);
	m_pointLight.SetPosition(glm::vec3(256.0f, 50.0f, 300.0f));
	m_pointLight.SetLightColour(glm::vec3(0.0f, 0.0f, 1.0f));

	m_terrain.InitTerrain();
	m_terrain.CreateTerrainWithPerlinNoise();

	Mouse2::Get().SetMouseVisible(false);
}
//-----------------------------------------------------------------------------
void GameApp::Update(float dt)
{
	resizeApp();

	m_mainCamera.UpdateLookAt();
	Player::Get().Update(m_mainCamera, m_terrain, dt);
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
	{

	}
	m_framebuffer.DeactivateFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);

		m_terrain.SetFog(true);
		m_terrain.Draw(m_mainCamera, &m_dirLight, &m_pointLight, Player::Get().GetSpotLight());
	}
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