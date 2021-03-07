#include "stdafx.h"
#include "RendererSystemTest.h"
#include "Engine.h"
#include "Player.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ShaderManager.h"
#include "PhysicsManager.h"
#include "Log.h"
#include "Other2/FileSystem.h"

const size_t NUM_OBJECTS = 10000;
//-----------------------------------------------------------------------------
EngineDescription RendererSystemTest::InitConfig()
{
	EngineDescription config;

	config.window.width = 1440;
	config.window.height = 900;
	return config;
}
//-----------------------------------------------------------------------------
RendererSystemTest::RendererSystemTest()
{
}
//-----------------------------------------------------------------------------
RendererSystemTest::~RendererSystemTest()
{
}
//-----------------------------------------------------------------------------
void RendererSystemTest::Init()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();

	cache = new ResourceCache();
	cache->AddResourceDir(FileSystem::ExecutableDir() + "Data");

	profiler = new Profiler();

	graphics = new Graphics();
	graphics->SetMode(IntVector2(1440, 900), false, true);

	renderer = new Renderer();
	renderer->SetupShadowMaps(1, 2048, FMT_D16);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3(0.0f, 20.0f, -45.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

#if 1

	for (int y = -5; y <= 5; ++y)
	{
		for (int x = -5; x <= 5; ++x)
		{
			StaticModel* object = scene->CreateChild<StaticModel>();
			object->SetPosition(Vector3(10.5f * x, -0.1f, 10.5f * y));
			object->SetScale(Vector3(10.0f, 0.1f, 10.0f));
			object->SetModel(cache->LoadResource<Model>("Box.mdl"));
			object->SetMaterial(cache->LoadResource<Material>("Stone.json"));
		}
	}

	for (unsigned i = 0; i < 435; ++i)
	{
		StaticModel* object = scene->CreateChild<StaticModel>();
		object->SetPosition(Vector3(Random() * 100.0f - 50.0f, 1.0f, Random() * 100.0f - 50.0f));
		object->SetScale(1.5f);
		object->SetModel(cache->LoadResource<Model>("Mushroom.mdl"));
		object->SetMaterial(cache->LoadResource<Material>("Mushroom.json"));
		object->SetCastShadows(true);
		object->SetLodBias(2.0f);
	}

	for (unsigned i = 0; i < 10; ++i)
	{
		Light* light = scene->CreateChild<Light>();
		light->SetLightType(LIGHT_POINT);
		light->SetCastShadows(true);
		Vector3 colorVec = 2.0f * Vector3(Random(), Random(), Random()).Normalized();
		light->SetColor(Color(colorVec.x, colorVec.y, colorVec.z));
		light->SetFov(90.0f);
		light->SetRange(20.0f);
		light->SetPosition(Vector3(Random() * 120.0f - 60.0f, 7.0f, Random() * 120.0f - 60.0f));
		light->SetDirection(Vector3(0.0f, -1.0f, 0.0f));
		light->SetShadowMapSize(256);
	}
#else
	StaticModel* object = scene->CreateChild<StaticModel>();
	object->SetPosition(Vector3(0.0f, -0.0f, 0.0f));
	object->SetScale(Vector3(10.0f, 0.1f, 10.0f));
	object->SetModel(cache->LoadResource<Model>("Box.mdl"));
	object->SetMaterial(cache->LoadResource<Material>("Stone.json"));

	StaticModel* object2 = scene->CreateChild<StaticModel>();
	object2->SetPosition(Vector3(0, 1.0f, 0));
	object2->SetScale(1.5f);
	object2->SetModel(cache->LoadResource<Model>("Mushroom.mdl"));
	object2->SetMaterial(cache->LoadResource<Material>("Mushroom.json"));
	object2->SetCastShadows(true);
	object2->SetLodBias(2.0f);

	Light* light = scene->CreateChild<Light>();
	light->SetLightType(LIGHT_POINT);
	light->SetCastShadows(true);
	Vector3 colorVec = 2.0f * Vector3(Random(), Random(), Random()).Normalized();
	light->SetColor(Color(colorVec.x, colorVec.y, colorVec.z));
	light->SetFov(90.0f);
	light->SetRange(20.0f);
	light->SetPosition(Vector3(0, 7.0f, 0));
	light->SetDirection(Vector3(0.0f, -1.0f, 0.0f));
	light->SetShadowMapSize(256);
#endif

	Mouse::Get().SetMouseVisible(false);
}
//-----------------------------------------------------------------------------
void RendererSystemTest::Update(float deltaTime)
{
	resizeApp();

	frameTimer.Reset();
	if (profilerTimer.ElapsedMSec() >= 1000)
	{
		profilerOutput = profiler->OutputResults();
		profilerTimer.Reset();
		profiler->BeginInterval();
	}

	profiler->BeginFrame();

	const float MouseDeltaX = static_cast<float>(Mouse::Get().GetMouseMove().x);
	const float MouseDeltaY = static_cast<float>(Mouse::Get().GetMouseMove().y);

	//pitch += MouseDeltaY * 0.25f;
	//yaw += MouseDeltaX * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));

	if (Keyboard::Get().KeyDown(Keyboard::KEY_W))
		camera->Translate(Vector3::FORWARD * deltaTime * 30.0f);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_S))
		camera->Translate(Vector3::BACK * deltaTime * 30.0f);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_A))
		camera->Translate(Vector3::LEFT * deltaTime * 30.0f);
	if (Keyboard::Get().KeyDown(Keyboard::KEY_D))
		camera->Translate(Vector3::RIGHT * deltaTime * 30.0f);

	// Update camera aspect ratio based on window size
	camera->SetAspectRatio((float)graphics->Width() / (float)graphics->Height());
}
//-----------------------------------------------------------------------------
void RendererSystemTest::ProcessInput(float dt)
{
}
//-----------------------------------------------------------------------------
void RendererSystemTest::Render()
{
	{
		PROFILE(RenderScene);
		Vector<PassDesc> passes;
		passes.Push(PassDesc("opaque", SORT_STATE, true));
		passes.Push(PassDesc("alpha", SORT_BACK_TO_FRONT, true));
		renderer->PrepareView(scene, camera, passes);

		renderer->RenderShadowMaps();
		graphics->ResetRenderTargets();
		graphics->ResetViewport();
		graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH, Color::BLACK);
		renderer->RenderBatches(passes);
	}
	graphics->Present();
	profiler->EndFrame();
}
//-----------------------------------------------------------------------------
void RendererSystemTest::Close()
{
	SE_LOG(profilerOutput.CString());
	graphics->Close();
}
//-----------------------------------------------------------------------------
void RendererSystemTest::resizeApp()
{
	if (m_width != GetEngineDescription().window.width || m_height != GetEngineDescription().window.height)
	{
		m_width = GetEngineDescription().window.width;
		m_height = GetEngineDescription().window.height;
	}
}
//-----------------------------------------------------------------------------