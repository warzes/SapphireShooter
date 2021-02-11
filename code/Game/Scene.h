#pragma once

#include "Skybox.h"
#include "Terrain2.h"
#include "InterfaceCamera.h"
#include "Light.h"
#include "ShadersManager.h"
#include "Shadow.h"

class Scene
{
	ShadersManager& manager;
	InterfaceCamera* camera = nullptr;
	Skybox* skybox = nullptr;
	std::vector<Renderable*> terrains;
	std::vector<Renderable*> objects;
	std::vector<Renderable*> animations;
	std::vector<Renderable*> waters;
	std::vector<Light*> lights;
	std::vector<Shadow> shadows;
	bool allowShadows = true;

	void initProgram(Program& program, const glm::vec4& clipPlane);
	void initPrograms(const glm::vec4& clipPlane);

	void renderTerrains(Program& prog);
	void renderObjects(Program& prog);
	void renderAnimations(Program& prog);
	void renderWaters(Program& prog);
	void renderLights();
	void renderSkybox();

public:
	Scene(InterfaceCamera* cam, ShadersManager& shadersmanager) : camera(cam), manager(shadersmanager) { ; }

	void renderShadows();
	virtual void render(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));

	void addSkybox(Skybox& obj) { skybox = &obj; }
	void addLight(Light& obj);
	void removeLight(const unsigned& n);

	void addTerrain(Renderable& obj) { terrains.push_back(&obj); }
	void addObject(Renderable& obj) { objects.push_back(&obj); }
	void addAnimation(Renderable& obj) { animations.push_back(&obj); }
	void addWater(Renderable& obj) { waters.push_back(&obj); }

	void removeTerrain(const unsigned& n) { terrains.erase(terrains.begin() + n); }
	void removeObject(const unsigned& n) { objects.erase(objects.begin() + n); }
	void removeAnimation(const unsigned& n) { animations.erase(animations.begin() + n); }
	void removeWater(const unsigned& n) { waters.erase(waters.begin() + n); }

	Skybox& getSkybox() { return *skybox; }
	std::vector<Renderable*>& getTerrains() { return terrains; }
	std::vector<Renderable*>& getObjects() { return objects; }
	std::vector<Renderable*>& getAnimations() { return animations; }
	std::vector<Renderable*>& getWaters() { return waters; }
	std::vector<Light*>& getLights() { return lights; }

	void setShadows(bool val) { allowShadows = val; }
	bool isAllowShadows() const { return allowShadows; }
};