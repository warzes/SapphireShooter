#pragma once

#include "Geometry.h"

// TODO: ������������ � ����������� Blur � � Water - �������

class Plane : public Geometry
{
public:
	Plane();

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void Render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};