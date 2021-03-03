#include "stdafx.h"
#include "TestApp.h"
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
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_mainCamera.SetCameraPos(glm::vec3(0.0f, 5.0f, 0.0f));
	m_mainCamera.Rotate(90, 0);

	//m_model.Init("../res/Models3D/swords/sword-0.obj", m_mainCamera, "../res/Shaders/SingleModelLoader.vs", "../res/Shaders/SingleModelLoader.fs", false);
	//m_model.SetSpotlight(false);

	//Mouse::Get().SetMouseVisible(false);

	//======================================================================================
	// NEW

	RegisterGraphicsLibrary();
	RegisterResourceLibrary();

	cache = new ResourceCache();
	cache->AddResourceDir(ExecutableDir() + "../Data");

	graphics = new Graphics();
	graphics->SetMode(IntVector2(1440, 900), false, true);

	float vertexData[] = 
	{
		// Position             // Texcoord
		0.0f, 0.05f, 0.0f,      0.5f, 0.0f,
		0.05f, -0.05f, 0.0f,    1.0f, 1.0f,
		-0.05f, -0.05f, 0.0f,   0.0f, 1.0f
	};
		
	Vector<VertexElement> vertexDeclaration;
	vertexDeclaration.Push(VertexElement(ELEM_VECTOR3, SEM_POSITION));
	vertexDeclaration.Push(VertexElement(ELEM_VECTOR2, SEM_TEXCOORD));
	vb = new VertexBuffer();
	vb->Define(USAGE_IMMUTABLE, 3, vertexDeclaration, true, vertexData);
		
	Vector<VertexElement> instanceVertexDeclaration;
	instanceVertexDeclaration.Push(VertexElement(ELEM_VECTOR3, SEM_TEXCOORD, 1, true));
	ivb = new VertexBuffer();
	ivb->Define(USAGE_DYNAMIC, NUM_OBJECTS, instanceVertexDeclaration, true);

	unsigned short indexData[] = 
	{
		0,
		1,
		2
	};

	ib = new IndexBuffer();
	ib->Define(USAGE_IMMUTABLE, 3, sizeof(unsigned short), true, indexData);
		
	Constant pc(ELEM_VECTOR4, "Color");
	pcb = new ConstantBuffer();
	pcb->Define(USAGE_IMMUTABLE, 1, &pc);
	pcb->SetConstant("Color", Color::WHITE);
	pcb->Apply();

	String vsCode =
#if SE_D3D11
		"struct VOut\n"
		"{\n"
		"    float4 position : SV_POSITION;\n"
		"    float2 texCoord : TEXCOORD0;\n"
		"};\n"
		"\n"
		"VOut main(float3 position : POSITION, float2 texCoord : TEXCOORD0, float3 objectPosition : TEXCOORD1)\n"
		"{\n"
		"    VOut output;\n"
		"    output.position = float4(position + objectPosition, 1.0);\n"
		"    output.texCoord = texCoord;\n"
		"    return output;\n"
		"}";
#else
		"#version 150\n"
		"\n"
		"in vec3 position;\n"
		"in vec2 texCoord;\n"
		"in vec3 texCoord1; // objectPosition\n"
		"\n"
		"out vec2 vTexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position + texCoord1, 1.0);\n"
		"    vTexCoord = texCoord;\n"
		"}\n";
#endif

	vs = new Shader2();
	vs->SetName("Test.vs");
	vs->Define(SHADER_VS, vsCode);
	vsv = vs->CreateVariation();

	String psCode =
#if SE_D3D11
		"cbuffer ConstantBuffer : register(b0)\n"
		"{\n"
		"    float4 color;\n"
		"}\n"
		"\n"
		"Texture2D Texture : register(t0);\n"
		"SamplerState Sampler : register(s0);\n"
		"\n"
		"float4 main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD0) : SV_TARGET\n"
		"{\n"
		"    return color * Texture.Sample(Sampler, texCoord);\n"
		"}\n";
#else
		"#version 150\n"
		"\n"
		"layout(std140) uniform ConstantBuffer0\n"
		"{\n"
		"    vec4 color;\n"
		"};\n"
		"\n"
		"uniform sampler2D Texture0;\n"
		"in vec2 vTexCoord;\n"
		"out vec4 fragColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    fragColor = color * texture(Texture0, vTexCoord);\n"
		"}\n";
#endif

	ps = new Shader2();
	ps->SetName("Test.ps");
	ps->Define(SHADER_PS, psCode);
	psv = ps->CreateVariation();

	tex = cache->LoadResource<Texture2>("Test.png");
}
//-----------------------------------------------------------------------------
void TestApp::Update(float deltaTime)
{
	resizeApp();
	m_mainCamera.Update();
}
//-----------------------------------------------------------------------------
void TestApp::ProcessInput(float dt)
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
}
//-----------------------------------------------------------------------------
void TestApp::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//m_model.Draw(m_mainCamera, glm::vec3(0.0f, 0, 10.0f), glm::vec3(1.0f), 0.0f, glm::vec3(6.0f));

	Vector3 *instanceData = new Vector3[NUM_OBJECTS];
	for (size_t i = 0; i < NUM_OBJECTS; ++i)
		instanceData[i] = Vector3(Random() * 2.0f - 1.0f, Random() * 2.0f - 1.0f, 0.0f);
	ivb->SetData(0, NUM_OBJECTS, instanceData);
	delete[] instanceData;

	glEnable(GL_DEPTH_TEST);
	graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH, Color(0.0f, 0.0f, 0.5f));
	graphics->SetVertexBuffer(0, vb);
	graphics->SetVertexBuffer(1, ivb);
	graphics->SetIndexBuffer(ib);
	graphics->SetConstantBuffer(SHADER_PS, 0, pcb);
	graphics->SetShaders(vsv, psv);
	graphics->SetTexture(0, tex);
	graphics->SetDepthState(CMP_LESS_EQUAL, true);
	graphics->SetColorState(BLEND_MODE_REPLACE);
	graphics->SetRasterizerState(CULL_BACK, FILL_SOLID);
	graphics->DrawIndexedInstanced(TRIANGLE_LIST, 0, 3, 0, 0, NUM_OBJECTS);
}
//-----------------------------------------------------------------------------
void TestApp::Close()
{
	graphics->Close();
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