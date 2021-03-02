#pragma once

#include "Geometry.h"

// TODO: используется в постэффекте Blur и в Water - удалить

class Plane : public Geometry
{
public:
	Plane();

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void Render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};