#pragma once

#include "Shape2.h"

class Plane : public Shape2
{
public:
	Plane();
	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void Render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};