#pragma once

#include "Shape2.h"

class Cube2 : public Shape2
{
public:
	Cube2();
	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};