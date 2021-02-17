#pragma once

#include "Renderable.h"

class Geometry : public Renderable
{
public:
	~Geometry() override;
	unsigned GetVAO() const override { return m_VAO; }

protected:
	unsigned m_VAO = 0;
	unsigned m_VBO = 0;
};