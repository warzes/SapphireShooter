#include "stdafx.h"
#include "Geometry.h"
//-----------------------------------------------------------------------------
Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}
//-----------------------------------------------------------------------------