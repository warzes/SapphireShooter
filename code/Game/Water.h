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
	Water(Camera* cam) : camera(cam) { RotateX(-90); }

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	void renderReflectAndRefract(Scene* scene);
	virtual unsigned GetVAO() const { return plane.GetVAO(); }

	Texture& getDudvMap() { return dudvMap; }
	Texture& getNormalMap() { return normalMap; }
};