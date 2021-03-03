#pragma once

#include "EngineDescription.h"

#include "Camera3D.h"
#include "FontRenderer.h"
#include "TextureGenerator2D.h"
#include "ConverterToNormalMap.h"
#include "Bloom.h"
#include "PostProcessing.h"
#include "Scene.h"
#include "Water.h"
#include "Camera3D.h"
#include "Framebuffer.h"
#include "Terrain.h"
#include "Model.h"
#include "Font.h"
#include "Billboard.h"
#include "TestAnims.h"
#include "Enemy.h"

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

	Camera3D m_mainCamera;
	Framebuffer m_framebuffer;
	Text m_texts;
	Billboard* m_test;
	Model m_model;

	TestAnims* player1 = nullptr;

	std::vector<Enemy*> m_enemies;
	int m_enemyCount = 30;
	float m_enemySpawnTimer = 0.0f;

	int m_width = 0;
	int m_height = 0;
};