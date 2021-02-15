#pragma once

#include "Skybox.h"
#include "Camera.h"
#include "Light.h"
#include "ShadersManager.h"
#include "Shadow.h"

class Scene
{
	ShadersManager& manager;
	Camera* camera = nullptr;
	Skybox* skybox = nullptr;
	std::vector<Renderable*> objects;
	std::vector<Renderable*> animations;
	std::vector<Renderable*> waters;
	std::vector<Light*> lights;
	std::vector<Shadow> shadows;
	bool allowShadows = true;

	void initProgram(std::shared_ptr<ShaderProgram> program, const glm::vec4& clipPlane);
	void initPrograms(const glm::vec4& clipPlane);

	void renderObjects(std::shared_ptr<ShaderProgram> prog);
	void renderAnimations(std::shared_ptr<ShaderProgram> prog);
	void renderWaters(std::shared_ptr<ShaderProgram> prog);
	void renderLights();
	void renderSkybox();

public:
	Scene(Camera* cam, ShadersManager& shadersmanager) : camera(cam), manager(shadersmanager) { ; }

	void renderShadows();
	void render(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));

	void render1(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));
	void render2(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));

	void addSkybox(Skybox& obj) { skybox = &obj; }
	void addLight(Light& obj);
	void removeLight(const unsigned& n);

	void addObject(Renderable& obj) { objects.push_back(&obj); }
	void addAnimation(Renderable& obj) { animations.push_back(&obj); }
	void addWater(Renderable& obj) { waters.push_back(&obj); }

	void removeObject(const unsigned& n) { objects.erase(objects.begin() + n); }
	void removeAnimation(const unsigned& n) { animations.erase(animations.begin() + n); }
	void removeWater(const unsigned& n) { waters.erase(waters.begin() + n); }

	Skybox& getSkybox() { return *skybox; }
	std::vector<Renderable*>& getObjects() { return objects; }
	std::vector<Renderable*>& getAnimations() { return animations; }
	std::vector<Renderable*>& getWaters() { return waters; }
	std::vector<Light*>& getLights() { return lights; }

	void setShadows(bool val) { allowShadows = val; }
	bool isAllowShadows() const { return allowShadows; }
};