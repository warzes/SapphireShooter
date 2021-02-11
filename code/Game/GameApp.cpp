#include "stdafx.h"
#include "GameApp.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse2.h"
#include "Keyboard.h"
#include "GLViewport.h"
#if TEST
#include "PerlinNoise2D.h"
#include "SmoothNoise2D.h"
#endif

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
#if TEST
	: camera(&perspectiveCamera)
	, pointLight(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(2.0f), glm::vec3(2.1f), glm::vec3(1.0f), 1.0f, 0.0f, 0.0f)
#endif
{
}
//-----------------------------------------------------------------------------
GameApp::~GameApp()
{
}
//-----------------------------------------------------------------------------
void GameApp::Init()
{
#if TEST
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	//glEnable(GL_CLIP_DISTANCE0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initShadersManager();

	bloomEffect = new Bloom();
	postHDR = new PostProcessing(2);
	post = new PostProcessing();

	camera->setPosition(glm::vec3(0.0f, 25.0f, 0.0f));
	camera->rotate(50, -80);

	fontRenderer = new FontRenderer(Font("fonts/arial.ttf"));
	fontRenderer->setColor(glm::vec3(1.0));

	skybox.init();

	skybox.getTexture().load({ 
		"textures/cubemapTEST/px.jpg", 
		"textures/cubemapTEST/nx.jpg", 
		"textures/cubemapTEST/py.jpg", 
		"textures/cubemapTEST/ny.jpg", 
		"textures/cubemapTEST/pz.jpg", 
		"textures/cubemapTEST/nz.jpg" });

	terrain.init();

	terrain.getGrassTexture().load("textures/grass.jpg", GL_TEXTURE_2D);
	terrain.getStoneTexture().load("textures/stone.jpg", GL_TEXTURE_2D);
	terrain.setPosition(glm::vec3(0.0f));
	terrain.scale(glm::vec3(400.0f));

	water = new Water(camera);

	water->getDudvMap().load("textures/water-dudv.jpg", GL_TEXTURE_2D);
	water->getNormalMap().load("textures/water-normal.jpg", GL_TEXTURE_2D);
	water->setPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	water->scale(glm::vec3(200.0f));

	scene = new Scene(camera, manager);
	//scene->addSkybox(skybox);
	//scene->addTerrain(terrain);
	//scene->addLight(pointLight);

	//generatePerlinPlain();
	generateSmooth256();

#else
	Player::Get().Init(m_mainCamera, glm::vec3(256.0f, 0.0f, 300.0f));

	m_dirLight.Configure(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	m_dirLight.SetDirection(glm::vec3(0.2f, 1.0f, 0.5f));
	m_dirLight.SetColour(glm::vec3(0.97f, 0.88f, 0.70f));

	m_pointLight.Configure(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045f, 0.0075f);
	m_pointLight.SetPosition(glm::vec3(256.0f, 50.0f, 300.0f));
	m_pointLight.SetLightColour(glm::vec3(0.0f, 0.0f, 1.0f));

	m_terrain.InitTerrain();
	m_terrain.CreateTerrainWithPerlinNoise();

	//m_jeep.Init("../res/Models3D/jeep/jeep.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	//m_jeep.SetSpotlight(true);

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
#endif
	Mouse2::Get().SetMouseVisible(false);
}
//-----------------------------------------------------------------------------
void GameApp::Update(float dt)
{
	resizeApp();
#if TEST
#else
	m_mainCamera.UpdateLookAt();
	Player::Get().Update(m_mainCamera, m_terrain, dt);
#endif
}
//-----------------------------------------------------------------------------
void GameApp::ProcessInput(float dt)
{
#if TEST
	if (firstTime)
	{
		//lastPosX = posX;
		//lastPosY = posY;
		firstTime = false;
	}
	float MouseDeltaX = static_cast<float>(Mouse2::Get().MouseMove().x);
	float MouseDeltaY = static_cast<float>(Mouse2::Get().MouseMove().y);

	camera->rotate(MouseDeltaX, MouseDeltaY);
	//camera->rotate(posX - lastPosX, lastPosY - posY);
	//lastPosX = posX;
	//lastPosY = posY;

	if (Keyboard::Get().KeyDown(Keyboard::KEY_W))
		camera->moveTop();
	if (Keyboard::Get().KeyDown(Keyboard::KEY_S))
		camera->moveBottom();
	if (Keyboard::Get().KeyDown(Keyboard::KEY_A))
		camera->moveLeft();
	if (Keyboard::Get().KeyDown(Keyboard::KEY_D))
		camera->moveRight();

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

	if (Keyboard::Get().KeyDown(Keyboard::KEY_NUMPAD_DECIMAL))
	{
		terrain.setDepth(terrain.getDepth() - 0.001);
		water->setPosition(glm::vec3(0.0f, water->getPosition().y - terrain.getDepth(), 0.0f));
	}
	if (Keyboard::Get().KeyDown(Keyboard::KEY_NUMPAD_ADD))
	{
		terrain.setDepth(terrain.getDepth() + 0.001);
		water->setPosition(glm::vec3(0.0f, water->getPosition().y + terrain.getDepth(), 0.0f));
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_T))
	{
		if (terrain.getTessLevel() > 1)
			terrain.setTessLevel(terrain.getTessLevel() - 1);
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_Y))
	{
		if (terrain.getTessLevel() < 64)
			terrain.setTessLevel(terrain.getTessLevel() + 1);
	}

	if (Keyboard::Get().KeyPressed(Keyboard::KEY_C))
	{
		scene->setShadows(true);
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_Z))
	{
		scene->setShadows(false);
	}

	if (Keyboard::Get().KeyPressed(Keyboard::KEY_1))
	{
		generatePerlinPlain();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_2))
	{
		generatePerlinLowLands();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_3))
	{
		generatePerlinHighLands();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_4))
	{
		generatePerlinLittleMountains();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_5))
	{
		generatePerlinMountains();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_6))
	{
		generateSmooth256();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_7))
	{
		generateSmooth128();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_8))
	{
		generateSmooth64();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_9))
	{
		generateSmooth32();
	}
	if (Keyboard::Get().KeyPressed(Keyboard::KEY_0))
	{
		generateSmooth16();
	}
#else
#endif
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
#if TEST
	//scene->renderShadows();

	//if (isWater)
	//	water->renderReflectAndRefract(scene);

	//if (!polygonMode)
	//	postHDR->startProcessing();

	/*if (isWater)
	{
		scene->addWater(*water);
		scene->render();
		scene->removeWater(0);
	}*/
	/*else*/ scene->render();

	if (!polygonMode)
	{
		//postHDR->endProcessing();
		//bloomEffect->blurTexture(manager.getBlurProgram(), postHDR->getResultTextures()[1]);

		std::vector<Texture> result;
		result.push_back(postHDR->getResultTextures()[0]);
		result.push_back(bloomEffect->getResultTexture());

		//post->startProcessing();
		//PostProcessing::renderToQuad(manager.getHDRProgram(), result);

		/*fontRenderer->setScale(0.5);
		fontRenderer->setPosition(8, 600);
		fontRenderer->setText("123");
		fontRenderer->render(manager.getFontProgram());

		fontRenderer->setScale(0.3);
		fontRenderer->setPosition(100, 500);
		fontRenderer->setText("Tessellation Level: " + std::to_string(terrain.getTessLevel()));
		fontRenderer->render(manager.getFontProgram());*/
		//post->endProcessing();
		//Program& prog = manager.getPostProcessProgram();
		//PostProcessing::renderToQuad(prog, post->getResultTextures());
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

		//m_jeep.Draw(m_mainCamera, glm::vec3(256.0f, m_terrain.GetHeightOfTerrain(256.0f, 300.0f), 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(0.08f));
		m_swords.Draw(m_mainCamera, glm::vec3(300.0f, m_terrain.GetHeightOfTerrain(300.0f, 270.0f)+10, 270.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));

		glDisable(GL_CULL_FACE);

		m_terrain.SetFog(true);
		m_terrain.Draw(m_mainCamera, &m_dirLight, &m_pointLight, Player::Get().GetSpotLight());

		//glm::mat4 model(1.0f);
		//glm::mat4 translation = glm::translate(glm::vec3(1.5f, 0.0f, -2.5f));
		//glm::mat4 rotation = glm::rotate(-0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 scaleMat = glm::scale(glm::vec3(2.0f, 2.0f, 1.0f));
		//glm::mat4 invViewMat = glm::inverse(m_mainCamera.GetViewMatrix());
		//model = invViewMat * translation * rotation * scaleMat;
		//m_test->Draw(model, m_mainCamera, glm::vec3(0.0f, 0.0f, 0.0f));

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
#if TEST
	delete postHDR;
	delete post;
	delete bloomEffect;
	delete fontRenderer;
	delete scene;
#else
	m_framebuffer.DestroyFramebuffer();
#endif
}
//-----------------------------------------------------------------------------
void GameApp::resizeApp()
{
	if (m_width != GetEngineDescription().window.width || m_height != GetEngineDescription().window.height)
	{
		m_width = GetEngineDescription().window.width;
		m_height = GetEngineDescription().window.height;		
#if TEST
		bloomEffect->updateBuffers();
		postHDR->updateBuffers();
		post->updateBuffers();
		water->buffers.updateBuffers();
#else		
		const float aspectRatio = float(m_width) / float(m_height);
		m_mainCamera.InitCameraPerspective(80.0f, aspectRatio, 0.1f, 5000.0f);
		
		m_framebuffer.DestroyFramebuffer();
		m_framebuffer.CreateFramebuffer(m_width, m_height);
#endif
	}
}
//-----------------------------------------------------------------------------
#if TEST
void GameApp::initShadersManager()
{
	Shader terrainVertex = Shader::CreateVertexShader("shaders/terrain/terrain.vs");
	Shader terrainTessControl = Shader::CreateTessalationControlShader("shaders/terrain/terrain_control.glsl");

	//Program depthTerrainProgram;
	//depthTerrainProgram.Create();
	//depthTerrainProgram.AttachShader(terrainVertex);
	//depthTerrainProgram.AttachShader(terrainTessControl);
	//depthTerrainProgram.AttachShader(Shader::CreateTessalationEvaluationShader("shaders/depth/terrain_eval_depth.glsl"));
	//depthTerrainProgram.AttachShader(Shader::CreateGeometryShader("shaders/depth/depth.gs"));
	//depthTerrainProgram.AttachShader(Shader::CreateFragmentShader("shaders/depth/depth.fs"));
	//depthTerrainProgram.Bind();

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL error code: " << err << std::endl;

	Program terrainProgram;
	terrainProgram.Create();
	//terrainProgram.AttachShader(terrainVertex);
	//terrainProgram.AttachShader(terrainTessControl);
	//terrainProgram.AttachShader(Shader::CreateTessalationEvaluationShader("shaders/terrain/terrain_eval.glsl"));
	//terrainProgram.AttachShader(Shader::CreateFragmentShader("shaders/terrain/terrain.fs"));
	terrainProgram.Bind();
	//GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
      std::cout<<"GL error code: "<<err<<std::endl;

	manager.setSkyboxProgram(Program("shaders/skybox/skybox.vs", "shaders/skybox/skybox.fs"));
	manager.setHDRProgram(Program("shaders/effect/hdr.vs", "shaders/effect/hdr.fs"));
	manager.setBlurProgram(Program("shaders/effect/blur.vs", "shaders/effect/blur.fs"));
	manager.setFontProgram(Program("shaders/font/font.vs", "shaders/font/font.fs"));
	manager.setPostProcessProgram(Program("shaders/effect/postprocessing.vs", "shaders/effect/postprocessing.fs"));
	manager.setWaterProgram(Program("shaders/water/water.vs", "shaders/water/water.fs"));
	manager.setTerrainProgram(terrainProgram);
	//manager.setTerrainDepthProgram(depthTerrainProgram);
}

void GameApp::generatePerlinPlain()
{
	static const float FREQUENT = 2;
	static const float AMPLITUDE = 2;
	static const float PERSISTENCE = 1;
	static const unsigned OCTAVES = 1;
	static unsigned offset = 1;
	++offset;

	generator.generate(terrain.getHeightMap(), PerlinNoise2D(FREQUENT, AMPLITUDE, PERSISTENCE, OCTAVES, PERLIN_MULTI, offset, offset), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generatePerlinLowLands()
{
	static const float FREQUENT = 3;
	static const float AMPLITUDE = 3;
	static const float PERSISTENCE = 2;
	static const unsigned OCTAVES = 2;
	static unsigned offset = 1;
	++offset;

	generator.generate(terrain.getHeightMap(), PerlinNoise2D(FREQUENT, AMPLITUDE, PERSISTENCE, OCTAVES, PERLIN_MULTI, offset, offset), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generatePerlinHighLands()
{
	static const float FREQUENT = 4;
	static const float AMPLITUDE = 4.5;
	static const float PERSISTENCE = 2.5;
	static const unsigned OCTAVES = 3;
	static unsigned offset = 1;
	++offset;

	generator.generate(terrain.getHeightMap(), PerlinNoise2D(FREQUENT, AMPLITUDE, PERSISTENCE, OCTAVES, PERLIN_MULTI, offset, offset), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generatePerlinLittleMountains()
{
	static const float FREQUENT = 10;
	static const float AMPLITUDE = 20;
	static const float PERSISTENCE = 0.5;
	static const unsigned OCTAVES = 5;
	static unsigned offset = 1;
	++offset;

	generator.generate(terrain.getHeightMap(), PerlinNoise2D(FREQUENT, AMPLITUDE, PERSISTENCE, OCTAVES, PERLIN_MULTI, offset, offset), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generatePerlinMountains()
{
	static const float FREQUENT = 20;
	static const float AMPLITUDE = 40;
	static const float PERSISTENCE = 0.45;
	static const unsigned OCTAVES = 6;
	static unsigned offset = 1;
	++offset;

	generator.generate(terrain.getHeightMap(), PerlinNoise2D(FREQUENT, AMPLITUDE, PERSISTENCE, OCTAVES, PERLIN_MULTI, offset, offset), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generateSmooth256()
{
	generator.generate(terrain.getHeightMap(), SmoothNoise2D(256), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generateSmooth128()
{
	generator.generate(terrain.getHeightMap(), SmoothNoise2D(128), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generateSmooth64()
{
	generator.generate(terrain.getHeightMap(), SmoothNoise2D(64), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generateSmooth32()
{
	generator.generate(terrain.getHeightMap(), SmoothNoise2D(32), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

void GameApp::generateSmooth16()
{
	generator.generate(terrain.getHeightMap(), SmoothNoise2D(16), TERRAIN_SIZE, TERRAIN_SIZE);
	conventer.convert(generator.getTextureData(), terrain.getNormalMap(), TERRAIN_SIZE, TERRAIN_SIZE);
}

#endif