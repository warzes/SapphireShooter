#pragma once

#include "Shape2.h"

class Cube2 : public Shape2
{
public:
	Cube2();
	virtual void render(Program& program);
	virtual void render(Program& program, const unsigned& amount);
};