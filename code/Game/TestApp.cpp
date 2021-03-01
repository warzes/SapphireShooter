#include "stdafx.h"
#include "TestApp.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ShaderManager.h"
#include "PhysicsManager.h"
#include "Log.h"

//-----------------------------------------------------------------------------
EngineDescription TestApp::InitConfig()
{
	EngineDescription config;

	config.window.width = 1440;
	config.window.height = 900;
	return config;
}
//-----------------------------------------------------------------------------
TestApp::TestApp()
{
}
//-----------------------------------------------------------------------------
TestApp::~TestApp()
{
}
//-----------------------------------------------------------------------------
void TestApp::Init()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_mainCamera.SetCameraPos(glm::vec3(0.0f, 5.0f, 0.0f));
	m_mainCamera.Rotate(90, 0);
#error "сделать чтобы камера могла смотреть в точку"
	https://www.youtube.com/user/UncleBobGmail/videos
	C#
	UE
	постапок?

	m_model.Init("../res/Models3D/swords/sword-0.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	m_model.SetSpotlight(false);

	Mouse::Get().SetMouseVisible(false);
}
//-----------------------------------------------------------------------------
void TestApp::Update(float deltaTime)
{
	resizeApp();
	m_mainCamera.UpdateLookAt();
}
//-----------------------------------------------------------------------------
void TestApp::ProcessInput(float dt)
{
	m_mainCamera.MouseUpdate(dt);

	if (Keyboard::Get().KeyDown(Keyboard::KEY_W))
		m_mainCamera.MoveForward(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_S))
		m_mainCamera.MoveBackward(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_A))
		m_mainCamera.StrafeLeft(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_D))
		m_mainCamera.StrafeRight(dt);
}
//-----------------------------------------------------------------------------
void TestApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_model.Draw(m_mainCamera, glm::vec3(0.0f, 0, 10.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));
}
//-----------------------------------------------------------------------------
void TestApp::Close()
{

}
//-----------------------------------------------------------------------------
void TestApp::resizeApp()
{
	if (m_width != GetEngineDescription().window.width || m_height != GetEngineDescription().window.height)
	{
		m_width = GetEngineDescription().window.width;
		m_height = GetEngineDescription().window.height;

		const float aspectRatio = float(m_width) / float(m_height);
		m_mainCamera.InitCameraPerspective(80.0f, aspectRatio, 0.1f, 5000.0f);
	}
}
//-----------------------------------------------------------------------------