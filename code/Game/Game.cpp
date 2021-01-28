#include "stdafx.h"
#include "Game.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
EngineDescription Game::InitConfig()
{
	EngineDescription config;
	return config;
}
//-----------------------------------------------------------------------------
Game::Game()
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
}
//-----------------------------------------------------------------------------
void Game::Init()
{
	float width = GetEngineDescription().window.width;
	float height = GetEngineDescription().window.height;

	loadResources();
	m_camera.InitCameraPerspective(80.0f, width / height, 0.1f, 5000.0f);
	m_cameraHUD.InitCameraOrthographic(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 100.0f);
}
//-----------------------------------------------------------------------------
void Game::Update(float dt)
{
}
//-----------------------------------------------------------------------------
void Game::ProcessInput(float dt)
{
}
//-----------------------------------------------------------------------------
void Game::Render()
{
}
//-----------------------------------------------------------------------------
void Game::Close()
{
}
//-----------------------------------------------------------------------------
void Game::loadResources()
{
}
//-----------------------------------------------------------------------------