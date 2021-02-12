#pragma once

#include "Transformationable.h"

class TransformDecorator : public Transformationable
{
	Renderable& renderer;

public:
	TransformDecorator(Renderable& object) : renderer(object) { ; }

	Renderable& getReference() { return renderer; }

	virtual void render(std::shared_ptr<ShaderProgram> program)
	{
		Transformationable::render(program);
		renderer.render(program);
	}
};