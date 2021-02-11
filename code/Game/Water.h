#pragma once

#include "Plane.h"
#include "WaterBuffers.h"
#include "Transformationable.h"
#include "InterfaceCamera.h"
#include "Scene.h"

class Water : public Transformationable
{
	Plane plane;
	Texture dudvMap;
	Texture normalMap;
	float offset = 0;
	InterfaceCamera* camera = nullptr;

public:
	WaterBuffers buffers;
	Water(InterfaceCamera* cam) : camera(cam) { rotateX(-90); }

	virtual void render(Program& program);
	void renderReflectAndRefract(Scene* scene);
	virtual unsigned getVAO() const { return plane.getVAO(); }

	Texture& getDudvMap() { return dudvMap; }
	Texture& getNormalMap() { return normalMap; }
};