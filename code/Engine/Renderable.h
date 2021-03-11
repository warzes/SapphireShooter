#pragma once

#if SE_OPENGL

#include "ShaderProgram.h"

class Renderable
{
public:
	virtual ~Renderable() = default;

	virtual void Render(std::shared_ptr<ShaderProgram> program) = 0;
	virtual unsigned GetVAO() const { return 0; }
};

#endif