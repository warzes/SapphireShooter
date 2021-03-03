#pragma once

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
#include "Other2/Renderer.h"
#include "Other2/Profiler.h"
#include "Other2/Octree.h"
#include "Other2/Scene.h"
#include "Other2/Model.h"
#include "Other2/StaticModel.h"
#include "Other2/Light.h"

class RendererSystemTest
{
public:
	RendererSystemTest();
	~RendererSystemTest();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void resizeApp();

	int m_width = 0;
	int m_height = 0;
	// new
	AutoPtr<ResourceCache> cache;
	AutoPtr<Graphics> graphics;
	AutoPtr<Renderer> renderer;
	AutoPtr<Profiler> profiler;

	Camera* camera;

	SharedPtr<Scene> scene;

	Timer profilerTimer;
	HiresTimer frameTimer;
	String profilerOutput;
	float yaw = 0.0f, pitch = 20.0f;

};