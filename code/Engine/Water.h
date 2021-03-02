#pragma once

#include "Plane.h"
#include "WaterBuffers.h"
#include "Transformationable.h"
#include "Camera3D.h"
#include "ShaderProgram.h"

class Scene;

class Water : public Transformationable
{
public:
	Water(Camera3D* cam) : m_camera(cam) { RotateX(-90); }

	void Create();

	void Render(std::shared_ptr<ShaderProgram> program) override;
	void RenderReflectAndRefract(Scene* scene);
	unsigned GetVAO() const  override { return m_plane.GetVAO(); }

	WaterBuffers buffers;

private:
	Plane m_plane;
	Texture m_dudvMap;
	Texture m_normalMap;
	float m_offset = 0;
	Camera3D* m_camera = nullptr;
};