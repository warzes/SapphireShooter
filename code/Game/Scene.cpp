#include "stdafx.h"
#include "Scene.h"
#include "Engine.h"

void Scene::initPrograms(const glm::vec4& clipPlane)
{
	initProgram(manager.getObjectProgram(), clipPlane);
	initProgram(manager.getAnimProgram(), clipPlane);
	initProgram(manager.getTerrainProgram(), clipPlane);
	initProgram(manager.getWaterProgram(), clipPlane);
	initProgram(manager.getTerrainDepthProgram(), clipPlane);
}

void Scene::initProgram(Program& program, const glm::vec4& clipPlane)
{
	if (program.GetId() != 0)
	{
		program.Bind();
		program.SetMat4("viewProject", camera->getViewProjectionMatrix());
		program.SetVec3("viewPos", camera->getPosition());
		program.SetInt("lightsAmount", Light::amount);
		program.SetInt("allowShadows", allowShadows ? 1 : 0);
		program.SetVec4("clipPlane", clipPlane);
	}
}

void Scene::renderShadows()
{
	if (allowShadows)
	{
		std::vector<Program*> programs;
		if (manager.getObjectProgram().GetId() != 0)
			programs.push_back(&manager.getObjectProgram());
		if (manager.getAnimProgram().GetId() != 0)
			programs.push_back(&manager.getAnimProgram());
		if (manager.getTerrainProgram().GetId() != 0)
			programs.push_back(&manager.getTerrainProgram());

		std::vector<Program*> depthPrograms;
		if (manager.getDepthProgram().GetId() != 0)
			depthPrograms.push_back(&manager.getDepthProgram());
		if (manager.getAnimDepthProgram().GetId() != 0)
			depthPrograms.push_back(&manager.getAnimDepthProgram());
		if (manager.getTerrainDepthProgram().GetId() != 0)
			depthPrograms.push_back(&manager.getTerrainDepthProgram());

		for (unsigned i = 0; i < lights.size(); ++i)
		{
			shadows[i].startCastShadow(*lights[i], depthPrograms);
			renderObjects(manager.getDepthProgram());
			renderAnimations(manager.getAnimDepthProgram());
			renderTerrains(manager.getTerrainDepthProgram());
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
	renderTerrains(manager.getTerrainProgram());
	renderObjects(manager.getObjectProgram());
	renderAnimations(manager.getAnimProgram());
	renderWaters(manager.getWaterProgram());
	renderSkybox();
}

void Scene::renderObjects(Program& prog)
{
	if (prog.GetId() != 0)
	{
		prog.Bind();
		for (unsigned i = 0; i < objects.size(); ++i)
			objects[i]->render(prog);
	}
}

void Scene::renderAnimations(Program& prog)
{
	if (prog.GetId() != 0)
	{
		prog.Bind();
		for (unsigned i = 0; i < animations.size(); ++i)
			animations[i]->render(prog);
	}
}

void Scene::renderWaters(Program& prog)
{
	if (prog.GetId() != 0)
	{
		prog.Bind();
		for (unsigned i = 0; i < waters.size(); ++i)
			waters[i]->render(prog);
	}
}

void Scene::renderTerrains(Program& prog)
{
	if (prog.GetId() != 0)
	{
		prog.Bind();
		for (unsigned i = 0; i < terrains.size(); ++i)
			terrains[i]->render(prog);
	}
}

void Scene::renderSkybox()
{
	Program& skyboxProgram = manager.getSkyboxProgram();
	if (skybox != nullptr && skyboxProgram.GetId() != 0)
	{
		skyboxProgram.Bind();
		glm::mat4 vp = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
		skyboxProgram.SetMat4("viewProject", vp);
		skybox->render(skyboxProgram);
	}
}

void Scene::renderLights()
{
	Program& objProgram = manager.getObjectProgram();
	Program& animProgram = manager.getAnimProgram();
	Program& worldProgram = manager.getTerrainProgram();
	Program& waterProgram = manager.getWaterProgram();
	bool isWorldProgram = worldProgram.GetId() != 0;
	bool isObjectProgram = objProgram.GetId() != 0;
	bool isAnimProgram = animProgram.GetId() != 0;
	bool isWaterProgram = waterProgram.GetId() != 0;

	for (unsigned i = 0; i < lights.size(); ++i)
	{
		if (isObjectProgram)
		{
			objProgram.Bind();
			lights[i]->render(objProgram);
		}

		if (isAnimProgram)
		{
			animProgram.Bind();
			lights[i]->render(animProgram);
		}

		if (isWaterProgram)
		{
			waterProgram.Bind();
			lights[i]->render(waterProgram);
		}

		if (isWorldProgram)
		{
			worldProgram.Bind();
			lights[i]->render(worldProgram);
		}
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