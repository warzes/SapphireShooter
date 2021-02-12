#pragma once

#include "ShaderProgram.h"

class Renderable
{
public:
	virtual void render(std::shared_ptr<ShaderProgram> program) = 0;
	virtual unsigned getVAO() const { return 0; }
	virtual ~Renderable() { ; }
};