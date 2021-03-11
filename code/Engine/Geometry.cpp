#include "stdafx.h"
#if SE_OPENGL
#include "Geometry.h"
//-----------------------------------------------------------------------------
Geometry2::~Geometry2()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}
//-----------------------------------------------------------------------------
#endif