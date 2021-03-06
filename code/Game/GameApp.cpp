#include "stdafx.h"
#if SE_OPENGL
#include "GameApp.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ShaderManager.h"
#include "PhysicsManager.h"
#include "Log.h"

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

	bloomEffect = new Bloom();
	postHDR = new PostProcessing(2);
	post = new PostProcessing();

	m_mainCamera.SetCameraPos(glm::vec3(30.0f, 25.0f, 30.0f));

	fontRenderer = new FontRenderer(Font("fonts/arial.ttf"));
	fontRenderer->setColor(glm::vec3(1.0));

	skybox.init();

	skybox.getTexture().load(
		{ 
		"textures/cubemap/px.jpg", 
		"textures/cubemap/nx.jpg", 
		"textures/cubemap/py.jpg", 
		"textures/cubemap/ny.jpg", 
		"textures/cubemap/pz.jpg", 
		"textures/cubemap/nz.jpg" 
		});

	water = new Water(&m_mainCamera);
	water->Create();
	water->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	water->SetScale(glm::vec3(2000.0f));

	scene = new Scene2(&m_mainCamera);
	scene->initShadersManager();
	scene->addSkybox(skybox);
	scene->addLight(pointLight);

	Player::Get().Init(m_mainCamera, glm::vec3(30.0f, 25.0f, 30.0f));

	m_model.Init("../res/Models3D/swords/sword-0.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	m_model.SetSpotlight(false);

	m_texts.Configure("../res/Fonts/Roboto-BoldItalic.ttf");
	m_texts.SetText("35");
	m_texts.SetScale(0.79f);
	m_texts.SetSpacing(0.79f);
	m_texts.SetPosition(glm::vec2(240.0f, 33.0f));
	
	std::vector<const char*> muzzleFlashShader{ "../res/Shaders/Muzzle Flash Shader/VertexShaderMuzzleFlash.vs", "../res/Shaders/Muzzle Flash Shader/FragmentShaderMuzzleFlash.fs" };
	m_test = Billboard::Create(ShapeType::Quad, "../res/Textures/axe.png", muzzleFlashShader, glm::vec3(2.0f, -2.5f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));	

	player1 = new TestAnims();
	player1->Translate(glm::vec3(5.0f, 40.0f, 5.0f));		
	//scene->addAnimation(*player1);

	for (unsigned int i = 0; i < 100; ++i)
	{
		Enemy* enemy = new Enemy(m_mainCamera, EnemyType::Range);
		m_enemies.push_back(enemy);
	}

	Mouse::Get().SetMouseVisible(false);
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime)
{
	resizeApp();

	m_mainCamera.Update();
	Player::Get().Update(m_mainCamera, scene->GetTerrain(), deltaTime);

	// Increase total number of enemies over time
	m_enemySpawnTimer += 1.0f * deltaTime;
	if (m_enemySpawnTimer >= 16.0f)
	{
		m_enemySpawnTimer = 0.0f;

		if (m_enemyCount < 100)
		{
			++m_enemyCount;
		}
	}

	// Update enemy units
	for (unsigned int i = 0; i < m_enemyCount; ++i)
	{
		m_enemies.at(i)->Update(scene->GetTerrain(), m_mainCamera, deltaTime);
	}

	// Update physics component
	PhysicsManager::Get().Update(m_mainCamera, deltaTime, m_enemies);
}
//-----------------------------------------------------------------------------
void GameApp::ProcessInput(float dt)
{
	m_mainCamera.MouseMoveUpdate(dt);

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
	scene->renderShadows();

	if (isWater)
		water->RenderReflectAndRefract(scene);

	if (!polygonMode)
		postHDR->startProcessing();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->render1();
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_model.Draw(m_mainCamera, glm::vec3(300.0f, scene->GetTerrain().GetHeightOfTerrain(300.0f, 270.0f) + 10, 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));

		// Draw enemy units
		for (unsigned int i = 0; i < m_enemyCount; ++i)
		{
			// Check if this enemy unit can respawn (if the data transfer is at 100, then this enemy cannot respawn anymore)
			m_enemies.at(i)->SetRespawnStatus(true);
			m_enemies.at(i)->Draw();
			m_enemies.at(i)->DrawShockwave();
		}

		glDisable(GL_CULL_FACE);
		//m_swords.Draw(m_mainCamera, glm::vec3(1.5f, -2.5f, -2.5f), glm::vec3(0.3f, 1.0f, 0.0f), 30.0f, glm::vec3(1.0f), true);
		glEnable(GL_CULL_FACE);
	}

	if (isWater)
	{
		glEnable(GL_BLEND);
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

		m_texts.Render();

		post->endProcessing();
		std::shared_ptr<ShaderProgram> prog = scene->getPostProcessProgram();
		PostProcessing::renderToQuad(prog, post->getResultTextures());
	}
}
//-----------------------------------------------------------------------------
void GameApp::Close()
{
	for (auto& i : m_enemies)
	{
		delete i;
		i = nullptr;
	}

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
#endif