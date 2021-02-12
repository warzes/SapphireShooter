#pragma once

#include "Shape2.h"

class Plane : public Shape2
{
public:
	Plane();
	virtual void render(std::shared_ptr<ShaderProgram> program);
	virtual void render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};