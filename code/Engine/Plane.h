#pragma once

#if SE_OPENGL

#include "Geometry.h"

// TODO: ������������ � ����������� Blur � � Water - �������

class Plane2 : public Geometry2
{
public:
	Plane2();

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void Render(std::shared_ptr<ShaderProgram> program, const unsigned& amount);
};

#endif