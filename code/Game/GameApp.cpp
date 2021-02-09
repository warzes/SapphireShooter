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

	m_jeep.Init("../res/Models3D/jeep/jeep.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	m_jeep.SetSpotlight(true);

	Text ammoText;
	ammoText.Configure("../res/Fonts/Roboto-BoldItalic.ttf");
	ammoText.SetText("35");
	ammoText.SetScale(0.79f);
	ammoText.SetSpacing(0.79f);
	ammoText.SetPosition(glm::vec2(240.0f, 33.0f));
	m_texts.push_back(ammoText);

	Text healthText;
	healthText.Configure("../res/Fonts/Roboto-BoldItalic.ttf");
	healthText.SetText("100");
	healthText.SetScale(0.79f);
	healthText.SetSpacing(0.79f);
	healthText.SetPosition(glm::vec2(110.0f, 33.0f));
	m_texts.push_back(healthText);

	Text dataTransmissionText;
	dataTransmissionText.Configure("../res/Fonts/Roboto-BoldItalic.ttf");
	dataTransmissionText.SetText("100%");
	dataTransmissionText.SetScale(0.5f);
	dataTransmissionText.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	dataTransmissionText.SetSpacing(0.7f);
	dataTransmissionText.SetPosition(glm::vec2(600.0f, 40.0f));
	m_texts.push_back(dataTransmissionText);

	std::vector<const char*> muzzleFlashShader{ "../res/Shaders/Muzzle Flash Shader/VertexShaderMuzzleFlash.vs", "../res/Shaders/Muzzle Flash Shader/FragmentShaderMuzzleFlash.fs" };
	m_test = Billboard::Create(ShapeType::Quad, "../res/Textures/muzzleFlash.png", muzzleFlashShader, glm::vec3(2.0f, -2.5f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

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

		m_jeep.Draw(m_mainCamera, glm::vec3(256.0f, m_terrain.GetHeightOfTerrain(256.0f, 300.0f), 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(0.08f));

		glDisable(GL_CULL_FACE);

		m_terrain.SetFog(true);
		m_terrain.Draw(m_mainCamera, &m_dirLight, &m_pointLight, Player::Get().GetSpotLight());


		//glm::mat4 model(1.0f);
		//glm::mat4 translation = glm::translate(glm::vec3(0.9f, -1.4f, -6.5f));
		//glm::mat4 rotation = glm::rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 scaleMat = glm::scale(glm::vec3(2.0f, 2.0f, 1.0f));
		//glm::mat4 invViewMat = glm::inverse(m_mainCamera.GetViewMatrix());
		//model = invViewMat * translation * rotation * scaleMat;
		//m_test->Draw(model, m_mainCamera, glm::vec3(0.0f, 0.0f, 0.0f));

		m_texts[0].SetText(std::to_string(100));
		m_texts[0].Render();
		m_texts[1].SetText(std::to_string(100));
		m_texts[1].Render();
		m_texts[2].SetText(std::to_string(100));
		m_texts[2].Render();
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