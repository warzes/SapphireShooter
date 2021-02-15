#pragma once

#define TEST 1

#include "EngineDescription.h"

#if TEST
#include "ShadersManager.h"
#include "Camera.h"
#include "FontRenderer.h"
#include "TextureGenerator2D.h"
#include "ConverterToNormalMap.h"
#include "Bloom.h"
#include "PostProcessing.h"
#include "Scene.h"
#include "Water.h"
//#else
#include "Camera.h"
#include "Framebuffer.h"
#include "Terrain.h"
#include "Model.h"
#include "Font.h"
#include "Billboard.h"
#endif

class GameApp
{
public:
	GameApp();
	~GameApp();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void resizeApp();
#if TEST
	const unsigned TERRAIN_SIZE = 512;
	const unsigned PERLIN_MULTI = 1;

	void initShadersManager();

	ShadersManager manager;
	FontRenderer *fontRenderer;
	TextureGenerator2D generator;
	ConverterToNormalMap conventer;
	Bloom *bloomEffect;
	PostProcessing *postHDR;
	PostProcessing *post;
	Scene* scene;
	Skybox skybox;
	Water *water;
	Light pointLight;
	bool firstTime = true;
	bool polygonMode = false;
	float lastPosX = 0;
	float lastPosY = 0;
	bool isWater = true;

	Camera m_mainCamera;

//#else
	//Camera m_mainCamera;
	Framebuffer m_framebuffer;
	Terrain m_terrain;
	DirectionalLight m_dirLight;
	PointLight m_pointLight;

	std::vector<Text> m_texts;

	Billboard* m_test;

	//Model m_jeep;
	Model m_swords;
#endif

	int m_width = 0;
	int m_height = 0;
};