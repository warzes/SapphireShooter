#pragma once

#include "Skybox.h"
#include "Camera3D.h"
#include "Light.h"
#include "Shadow.h"
#include "ShaderProgram.h"
#include "Water.h"
#include "Terrain.h"

class Scene
{
	Camera3D* camera = nullptr;
	Skybox* m_skybox = nullptr;
	std::vector<Renderable*> objects;
	std::vector<Renderable*> animations;
	std::vector<Water*> waters;
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

	std::shared_ptr<ShaderProgram> object = nullptr;
	std::shared_ptr<ShaderProgram> anim = nullptr;
	std::shared_ptr<ShaderProgram> depth = nullptr;
	std::shared_ptr<ShaderProgram> depthAnim = nullptr;
	std::shared_ptr<ShaderProgram> depthTerrain = nullptr;
	std::shared_ptr<ShaderProgram> hdr = nullptr;
	std::shared_ptr<ShaderProgram> blur = nullptr;
	std::shared_ptr<ShaderProgram> particle = nullptr;
	std::shared_ptr<ShaderProgram> skybox = nullptr;
	std::shared_ptr<ShaderProgram> font = nullptr;
	std::shared_ptr<ShaderProgram> postprocess = nullptr;
	std::shared_ptr<ShaderProgram> water = nullptr;

public:
	Scene(Camera3D* cam);

	void initShadersManager();

	void renderShadows();
	void render(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));

	void render1(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));
	void render2(const glm::vec4& clipPlane = glm::vec4(0, 1, 0, 10000));

	void addSkybox(Skybox& obj) { m_skybox = &obj; }
	void addLight(Light& obj);
	void removeLight(const unsigned& n);

	void addObject(Renderable& obj) { objects.push_back(&obj); }
	void addAnimation(Renderable& obj) { animations.push_back(&obj); }
	void addWater(Water& obj) { waters.push_back(&obj); }

	void removeObject(const unsigned& n) { objects.erase(objects.begin() + n); }
	void removeAnimation(const unsigned& n) { animations.erase(animations.begin() + n); }
	void removeWater(const unsigned& n) { waters.erase(waters.begin() + n); }

	Skybox& getSkybox() { return *m_skybox; }
	std::vector<Renderable*>& getObjects() { return objects; }
	std::vector<Renderable*>& getAnimations() { return animations; }
	std::vector<Water*>& getWaters() { return waters; }
	std::vector<Light*>& getLights() { return lights; }

	void setShadows(bool val) { allowShadows = val; }
	bool isAllowShadows() const { return allowShadows; }

	void setObjectProgram(const std::shared_ptr<ShaderProgram> prog) { object = prog; }
	void setAnimProgram(const std::shared_ptr<ShaderProgram> prog) { anim = prog; }
	void setDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depth = prog; }
	void setAnimDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depthAnim = prog; }
	void setTerrainDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depthTerrain = prog; }
	void setHDRProgram(const std::shared_ptr<ShaderProgram> prog) { hdr = prog; }
	void setPostProcessProgram(const std::shared_ptr<ShaderProgram> prog) { postprocess = prog; }
	void setBlurProgram(const std::shared_ptr<ShaderProgram> prog) { blur = prog; }
	void setParticleProgram(const std::shared_ptr<ShaderProgram> prog) { particle = prog; }
	void setSkyboxProgram(const std::shared_ptr<ShaderProgram> prog) { skybox = prog; }
	void setFontProgram(const std::shared_ptr<ShaderProgram> prog) { font = prog; }
	void setWaterProgram(const std::shared_ptr<ShaderProgram> prog) { water = prog; }

	std::shared_ptr<ShaderProgram> getObjectProgram() { return object; }
	std::shared_ptr<ShaderProgram> getAnimProgram() { return anim; }
	std::shared_ptr<ShaderProgram> getDepthProgram() { return depth; }
	std::shared_ptr<ShaderProgram> getAnimDepthProgram() { return depthAnim; }
	std::shared_ptr<ShaderProgram> getTerrainDepthProgram() { return depthTerrain; }
	std::shared_ptr<ShaderProgram> getHDRProgram() { return hdr; }
	std::shared_ptr<ShaderProgram> getPostProcessProgram() { return postprocess; }
	std::shared_ptr<ShaderProgram> getBlurProgram() { return blur; }
	std::shared_ptr<ShaderProgram> getParticleProgram() { return particle; }
	std::shared_ptr<ShaderProgram> getSkyboxProgram() { return skybox; }
	std::shared_ptr<ShaderProgram> getFontProgram() { return font; }
	std::shared_ptr<ShaderProgram> getWaterProgram() { return water; }

	Terrain& GetTerrain() { return m_terrain; }

private:
	Terrain m_terrain;
	Terrain m_terrain2;

	DirectionalLight m_dirLight;
	PointLight m_pointLight;
};