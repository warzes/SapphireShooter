#pragma once

#include "Renderable.h"

class Geometry2 : public Renderable
{
public:
	~Geometry2() override;
	unsigned GetVAO() const override { return m_VAO; }

protected:
	unsigned m_VAO = 0;
	unsigned m_VBO = 0;
};