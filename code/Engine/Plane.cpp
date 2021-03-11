#include "stdafx.h"
#if SE_OPENGL
#include "Plane.h"
//-----------------------------------------------------------------------------
Plane2::Plane2()
{
	static const float planeVertices[] = 
	{
		-1.0,  1.0, 0.0, 0.0, 1.0,
		-1.0, -1.0, 0.0, 0.0, 0.0,
		1.0,  1.0, 0.0, 1.0, 1.0,
		1.0, -1.0, 0.0, 1.0, 0.0
	};
	static const unsigned size = 5 * sizeof(float);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void Plane2::Render(std::shared_ptr<ShaderProgram> program)
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void Plane2::Render(std::shared_ptr<ShaderProgram> program, const unsigned& amount)
{
	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, amount);
	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
#endif