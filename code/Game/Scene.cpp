#include "stdafx.h"
#include "Scene.h"
#include "Engine.h"

void Scene::initPrograms(const glm::vec4& clipPlane)
{
	initProgram(manager.getObjectProgram(), clipPlane);
	initProgram(manager.getAnimProgram(), clipPlane);
	initProgram(manager.getWaterProgram(), clipPlane);
	initProgram(manager.getTerrainDepthProgram(), clipPlane);
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
		if (manager.getObjectProgram() != nullptr && manager.getObjectProgram()->GetId() != 0)
			programs.push_back(manager.getObjectProgram());
		if (manager.getAnimProgram() != nullptr && manager.getAnimProgram()->GetId() != 0)
			programs.push_back(manager.getAnimProgram());

		std::vector<std::shared_ptr<ShaderProgram>> depthPrograms;
		if (manager.getDepthProgram() != nullptr && manager.getDepthProgram()->GetId() != 0)
			depthPrograms.push_back(manager.getDepthProgram());
		if (manager.getAnimDepthProgram() != nullptr && manager.getAnimDepthProgram()->GetId() != 0)
			depthPrograms.push_back(manager.getAnimDepthProgram());
		if (manager.getTerrainDepthProgram() != nullptr && manager.getTerrainDepthProgram()->GetId() != 0)
			depthPrograms.push_back(manager.getTerrainDepthProgram());

		for (unsigned i = 0; i < lights.size(); ++i)
		{
			shadows[i].startCastShadow(*lights[i], depthPrograms);
			renderObjects(manager.getDepthProgram());
			renderAnimations(manager.getAnimDepthProgram());
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
	renderObjects(manager.getObjectProgram());
	renderAnimations(manager.getAnimProgram());
	renderWaters(manager.getWaterProgram());
	renderSkybox();
}

void Scene::renderObjects(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < objects.size(); ++i)
			objects[i]->render(prog);
	}
}

void Scene::renderAnimations(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < animations.size(); ++i)
			animations[i]->render(prog);
	}
}

void Scene::renderWaters(std::shared_ptr<ShaderProgram> prog)
{
	if (prog && prog->GetId() != 0)
	{
		prog->Bind();
		for (unsigned i = 0; i < waters.size(); ++i)
			waters[i]->render(prog);
	}
}

void Scene::renderSkybox()
{
	std::shared_ptr<ShaderProgram> skyboxProgram = manager.getSkyboxProgram();
	if (skybox != nullptr && skyboxProgram->GetId() != 0)
	{
		skyboxProgram->Bind();
		glm::mat4 vp = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix()));
		skyboxProgram->SetMatrix4("viewProject", vp);
		skybox->render(skyboxProgram);
	}
}

void Scene::renderLights()
{
	std::shared_ptr<ShaderProgram> objProgram = manager.getObjectProgram();
	std::shared_ptr<ShaderProgram> animProgram = manager.getAnimProgram();
	//std::shared_ptr<ShaderProgram> worldProgram = manager.getTerrainProgram();
	std::shared_ptr<ShaderProgram> waterProgram = manager.getWaterProgram();
	//bool isWorldProgram = worldProgram && worldProgram->GetId() != 0;
	bool isObjectProgram = objProgram && objProgram->GetId() != 0;
	bool isAnimProgram = animProgram && animProgram->GetId() != 0;
	bool isWaterProgram = waterProgram && waterProgram->GetId() != 0;

	for (unsigned i = 0; i < lights.size(); ++i)
	{
		if (isObjectProgram)
		{
			objProgram->Bind();
			lights[i]->render(objProgram);
		}

		if (isAnimProgram)
		{
			animProgram->Bind();
			lights[i]->render(animProgram);
		}

		if (isWaterProgram)
		{
			waterProgram->Bind();
			lights[i]->render(waterProgram);
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