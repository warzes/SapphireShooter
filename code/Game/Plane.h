#pragma once

#include "Shape2.h"

class Plane : public Shape2
{
public:
	Plane();
	virtual void render(Program& program);
	virtual void render(Program& program, const unsigned& amount);
};