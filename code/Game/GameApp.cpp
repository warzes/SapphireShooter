#include "stdafx.h"
#include "GameApp.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse2.h"
#include "Keyboard.h"
#include "GLViewport.h"
#include "ShaderManager.h"

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
	: pointLight(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(2.0f), glm::vec3(2.1f), glm::vec3(1.0f), 1.0f, 0.0f, 0.0f)
{
}
//-----------------------------------------------------------------------------
GameApp::~GameApp()
{
}
//-----------------------------------------------------------------------------
void GameApp::Init()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initShadersManager();

	bloomEffect = new Bloom();
	postHDR = new PostProcessing(2);
	post = new PostProcessing();

	m_mainCamera.SetCameraPos(glm::vec3(30.0f, 25.0f, 30.0f));

	fontRenderer = new FontRenderer(Font("fonts/arial.ttf"));
	fontRenderer->setColor(glm::vec3(1.0));

	skybox.init();

	skybox.getTexture().load(
		{ 
		"textures/cubemapTEST/px.jpg", 
		"textures/cubemapTEST/nx.jpg", 
		"textures/cubemapTEST/py.jpg", 
		"textures/cubemapTEST/ny.jpg", 
		"textures/cubemapTEST/pz.jpg", 
		"textures/cubemapTEST/nz.jpg" 
		});

	water = new Water(&m_mainCamera);

	water->getDudvMap().load("textures/water-dudv.jpg", GL_TEXTURE_2D);
	water->getNormalMap().load("textures/water-normal.jpg", GL_TEXTURE_2D);
	water->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	water->SetScale(glm::vec3(2000.0f));

	scene = new Scene(&m_mainCamera);
	scene->initShadersManager();
	scene->addSkybox(skybox);
	scene->addLight(pointLight);

	Player::Get().Init(m_mainCamera, glm::vec3(30.0f, 25.0f, 30.0f));

	m_dirLight.Configure(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	m_dirLight.SetDirection(glm::vec3(0.2f, 1.0f, 0.5f));
	m_dirLight.SetColour(glm::vec3(0.97f, 0.88f, 0.70f));

	m_pointLight.Configure(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045f, 0.0075f);
	m_pointLight.SetPosition(glm::vec3(256.0f, 50.0f, 300.0f));
	m_pointLight.SetLightColour(glm::vec3(0.0f, 0.0f, 1.0f));

	m_terrain.InitTerrain();
	m_terrain.CreateTerrainWithPerlinNoise();

	m_swords.Init("../res/Models3D/swords/sword-0.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	m_swords.SetSpotlight(false);

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
	m_test = Billboard::Create(ShapeType::Quad, "../res/Textures/axe.png", muzzleFlashShader, glm::vec3(2.0f, -2.5f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));	

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
	m_mainCamera.MouseUpdate(dt);

	if (Keyboard::Get().KeyDown(Keyboard::KEY_W))
		m_mainCamera.MoveForward(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_S))
		m_mainCamera.MoveBackward(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_A))
		m_mainCamera.StrafeLeft(dt);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_D))
		m_mainCamera.StrafeRight(dt);

	if (Keyboard::Get().KeyPressed(Keyboard::KEY_P))
	{
		polygonMode = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_O))
	{
		polygonMode = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (Keyboard::Get().KeyPressed(Keyboard::KEY_N))
	{
		isWater = false;
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_M))
	{
		isWater = true;
	}

	if (Keyboard::Get().KeyPressed(Keyboard::KEY_C))
	{
		scene->setShadows(true);
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_Z))
	{
		scene->setShadows(false);
	}
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
#if TEST
	scene->renderShadows();

	if (isWater)
		water->renderReflectAndRefract(scene);

	if (!polygonMode)
		postHDR->startProcessing();
	GLViewport::Clear(true, true);

	scene->render1();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_swords.Draw(m_mainCamera, glm::vec3(300.0f, m_terrain.GetHeightOfTerrain(300.0f, 270.0f) + 10, 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));
	glDisable(GL_CULL_FACE);
	m_terrain.SetFog(true);
	m_terrain.Draw(m_mainCamera, &m_dirLight, &m_pointLight, Player::Get().GetSpotLight());
	m_swords.Draw(m_mainCamera, glm::vec3(1.5f, -2.5f, -2.5f), glm::vec3(0.3f, 1.0f, 0.0f), 30.0f, glm::vec3(1.0f), true);
	glEnable(GL_CULL_FACE);

	if (isWater)
	{
		scene->addWater(*water);
		scene->render2();
		scene->removeWater(0);
	}

	if (!polygonMode)
	{
		postHDR->endProcessing();
		bloomEffect->blurTexture(scene->getBlurProgram(), postHDR->getResultTextures()[1]);

		std::vector<Texture> result;
		result.push_back(postHDR->getResultTextures()[0]);
		result.push_back(bloomEffect->getResultTexture());

		post->startProcessing();
		PostProcessing::renderToQuad(scene->getHDRProgram(), result);

		fontRenderer->setScale(0.5);
		fontRenderer->setPosition(8, 600);
		fontRenderer->setText("123");
		fontRenderer->Render(scene->getFontProgram());

		post->endProcessing();
		std::shared_ptr<ShaderProgram> prog = scene->getPostProcessProgram();
		PostProcessing::renderToQuad(prog, post->getResultTextures());
	}
#else
	m_framebuffer.ActivateFramebuffer();
	GLViewport::Clear(true, true);
	// TODO: render to texture
	{

	}
	m_framebuffer.DeactivateFramebuffer();
	GLViewport::Clear(true, true);
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		m_swords.Draw(m_mainCamera, glm::vec3(300.0f, m_terrain.GetHeightOfTerrain(300.0f, 270.0f)+10, 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));

		glDisable(GL_CULL_FACE);

		m_terrain.SetFog(true);
		m_terrain.Draw(m_mainCamera, &m_dirLight, &m_pointLight, Player::Get().GetSpotLight());

		// TODO: передавать матрицы
		m_swords.Draw(m_mainCamera, glm::vec3(1.5f, -2.5f, -2.5f), glm::vec3(0.3f, 1.0f, 0.0f), 30.0f, glm::vec3(1.0f), true);

		m_texts[0].SetText(std::to_string(100));
		m_texts[0].Render();
		m_texts[1].SetText(std::to_string(100));
		m_texts[1].Render();
		m_texts[2].SetText(std::to_string(100));
		m_texts[2].Render();
	}
#endif
}
//-----------------------------------------------------------------------------
void GameApp::Close()
{
	delete postHDR;
	delete post;
	delete bloomEffect;
	delete fontRenderer;
	delete scene;
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

		bloomEffect->updateBuffers();
		postHDR->updateBuffers();
		post->updateBuffers();
		water->buffers.updateBuffers();

		m_framebuffer.DestroyFramebuffer();
		m_framebuffer.CreateFramebuffer(m_width, m_height);
	}
}
//-----------------------------------------------------------------------------