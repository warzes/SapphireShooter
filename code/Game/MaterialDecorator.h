#pragma once

#include "Material.h"

class MaterialDecorator : public Renderable
{
	Renderable& renderer;
	Material material;

public:
	MaterialDecorator(Renderable& object) : renderer(object) { ; }
	MaterialDecorator(Renderable& object, const Material& mat) : renderer(object), material(mat) { ; }

	void setMaterial(const Material& mat) { material = mat; }
	const Material& getMaterial() { return material; }
	Renderable& getReference() { return renderer; }

	virtual void render(std::shared_ptr<ShaderProgram> program)
	{
		material.render(program);
		renderer.render(program);
	}
};