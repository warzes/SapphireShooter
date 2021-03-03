#pragma once

#define TEST 1

#include "EngineDescription.h"
#include "Camera3D.h"
#include "Model.h"
#include "Other2/ResourceCache.h"
#include "Other2/Graphics.h"
#include "Other2/VertexBuffer.h"
#include "Other2/IndexBuffer.h"
#include "Other2/ConstantBuffer.h"
#include "Other2/Shader.h"
#include "Other2/Texture.h"
#include "Other2/Random.h"

class TestApp
{
public:
	TestApp();
	~TestApp();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void resizeApp();

	Camera3D m_mainCamera;

	Model m_model;

	int m_width = 0;
	int m_height = 0;


	// new
	AutoPtr<ResourceCache> cache;
	AutoPtr<Graphics> graphics;
		
	SharedPtr<VertexBuffer> ivb;
	AutoPtr<VertexBuffer> vb;
	AutoPtr<IndexBuffer> ib;
	AutoPtr<ConstantBuffer> pcb;	
	AutoPtr<Shader2> vs;
	AutoPtr<Shader2> ps;
	ShaderVariation* vsv;
	ShaderVariation* psv;
	Texture2* tex;
};