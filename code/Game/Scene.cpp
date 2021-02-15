#include "stdafx.h"
#include "Scene.h"
#include "Engine.h"
#include "ShaderManager.h"

void Scene::initPrograms(const glm::vec4& clipPlane)
{
	initProgram(getObjectProgram(), clipPlane);
	initProgram(getAnimProgram(), clipPlane);
	initProgram(getWaterProgram(), clipPlane);
	initProgram(getTerrainDepthProgram(), clipPlane);
}

void Scene::initProgram(std::shared_ptr<ShaderProgram> program, const glm::vec4& clipPlane)
{
	if (program && program->GetId() != 0)
	{
		program->Bind();
		program->SetMatrix4("viewProject", camera->GetViewProjectionMatrix());
		program->SetVector3f("viewPos", camera->GetCameraPos());
		program->SetInteger("lightsAmount", Light::amount);
		program->SetInteger("allowShadows", allowShadows ? 1 : 0);
		program->SetVector4f("clipPlane", clipPlane);
	}
}

void Scene::renderShadows()
{
	if (allowShadows)
	{
		std::vector<std::shared_ptr<ShaderProgram>> programs;
		if (getObjectProgram() != nullptr && getObjectProgram()->GetId() != 0)
			programs.push_back(getObjectProgram());
		if (getAnimProgram() != nullptr && getAnimProgram()->GetId() != 0)
			programs.push_back(getAnimProgram());

		std::vector<std::shared_ptr<ShaderProgram>> depthPrograms;
		if (getDepthProgram() != nullptr && getDepthProgram()->GetId() != 0)
			depthPrograms.push_back(getDepthProgram());
		if (getAnimDepthProgram() != nullptr && getAnimDepthProgram()->GetId() != 0)
			depthPrograms.push_back(getAnimDepthProgram());
		if (getTerrainDepthProgram() != nullptr && getTerrainDepthProgram()->GetId() != 0)
			depthPrograms.push_back(getTerrainDepthProgram());

		for (unsigned i = 0; i < lights.size(); ++i)
		{
			shadows[i].startCastShadow(*lights[i], depthPrograms);
			renderObjects(getDepthProgram());
			renderAnimations(getAnimDepthProgram());
			shadows[i].endCastShadow(*lights[i], programs);
		}
	}
}

void Scene::render(const glm::vec4& clipPlane)
{
	auto width = GetEngineDescription().window.width;
	auto height = GetEngineDescription().window.height;
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	initPrograms(clipPlane);
	renderLights();
	renderObjects(getObjectProgram());
	renderAnimations(getAnimProgram());
	renderWaters(getWaterProgram());
	renderSkybox();
}

void Scene::render1(const glm::vec4& clipPlane)
{
	auto width = GetEngineDescription().window.width;
	auto height = GetEngineDescription().window.height;
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);

	initPrograms(clipPlane);
	renderLights();
	renderObjects(getObjectProgram());
	renderAnimations(getAnimProgram());
}

void Scene::render2(const glm::vec4& clipPlane)
{
	renderWaters(getWaterProgram());
	renderSkybox();
}

void Scene::renderObjects(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < objects.size(); ++i)
			objects[i]->Render(prog);
	}
}

void Scene::renderAnimations(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < animations.size(); ++i)
			animations[i]->Render(prog);
	}
}

void Scene::renderWaters(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < waters.size(); ++i)
			waters[i]->Render(prog);
	}
}

void Scene::renderSkybox()
{
	std::shared_ptr<ShaderProgram> skyboxProgram = getSkyboxProgram();
	if (skybox != nullptr && skyboxProgram->GetId() != 0)
	{
		skyboxProgram->Bind();
		glm::mat4 vp = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix()));
		skyboxProgram->SetMatrix4("viewProject", vp);
		m_skybox->Render(skyboxProgram);
	}
}

void Scene::renderLights()
{
	std::shared_ptr<ShaderProgram> objProgram = getObjectProgram();
	std::shared_ptr<ShaderProgram> animProgram = getAnimProgram();
	//std::shared_ptr<ShaderProgram> worldProgram = manager.getTerrainProgram();
	std::shared_ptr<ShaderProgram> waterProgram = getWaterProgram();
	//bool isWorldProgram = worldProgram && worldProgram->GetId() != 0;
	bool isObjectProgram = objProgram && objProgram->GetId() != 0;
	bool isAnimProgram = animProgram && animProgram->GetId() != 0;
	bool isWaterProgram = waterProgram && waterProgram->GetId() != 0;

	for (unsigned i = 0; i < lights.size(); ++i)
	{
		if (isObjectProgram)
		{
			objProgram->Bind();
			lights[i]->Render(objProgram);
		}

		if (isAnimProgram)
		{
			animProgram->Bind();
			lights[i]->Render(animProgram);
		}

		if (isWaterProgram)
		{
			waterProgram->Bind();
			lights[i]->Render(waterProgram);
		}

		//if (isWorldProgram)
		//{
		//	worldProgram->Bind();
		//	lights[i]->render(worldProgram);
		//}
	}
}

void Scene::addLight(Light& obj)
{
	shadows.push_back(Shadow());
	lights.push_back(&obj);
}

void Scene::removeLight(const unsigned& n)
{
	shadows.erase(shadows.begin() + n);
	lights.erase(lights.begin() + n);
}

void Scene::initShadersManager()
{
	setSkyboxProgram(ShaderManager::Get().LoadShader("skybox", "shaders/skybox/skybox.vs", "shaders/skybox/skybox.fs"));
	setHDRProgram(ShaderManager::Get().LoadShader("hdr", "shaders/effect/hdr.vs", "shaders/effect/hdr.fs"));
	setBlurProgram(ShaderManager::Get().LoadShader("blur", "shaders/effect/blur.vs", "shaders/effect/blur.fs"));
	setFontProgram(ShaderManager::Get().LoadShader("font", "shaders/font/font.vs", "shaders/font/font.fs"));
	setPostProcessProgram(ShaderManager::Get().LoadShader("postprocessing", "shaders/effect/postprocessing.vs", "shaders/effect/postprocessing.fs"));
	setWaterProgram(ShaderManager::Get().LoadShader("water", "shaders/water/water.vs", "shaders/water/water.fs"));
}