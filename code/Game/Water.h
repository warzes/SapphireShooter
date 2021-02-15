#pragma once

#include "Plane.h"
#include "WaterBuffers.h"
#include "Transformationable.h"
#include "Camera.h"
#include "Scene.h"

class Water : public Transformationable
{
	Plane plane;
	Texture dudvMap;
	Texture normalMap;
	float offset = 0;
	Camera* camera = nullptr;

public:
	WaterBuffers buffers;
	Water(Camera* cam) : camera(cam) { rotateX(-90); }

	virtual void render(std::shared_ptr<ShaderProgram> program);
	void renderReflectAndRefract(Scene* scene);
	virtual unsigned getVAO() const { return plane.getVAO(); }

	Texture& getDudvMap() { return dudvMap; }
	Texture& getNormalMap() { return normalMap; }
};