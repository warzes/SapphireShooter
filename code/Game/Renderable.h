#pragma once

#include "Program.h"

class Renderable
{
public:
	virtual void render(Program& program) = 0;
	virtual unsigned getVAO() const { return 0; }
	virtual ~Renderable() { ; }
};