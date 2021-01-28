#include "stdafx.h"
#include "OpenGLStateCache.h"
#include "OGLFunc.h"
//-----------------------------------------------------------------------------
void OpenGLStateCache::UseProgram(GLuint program)
{
#if SE_OGL_CASHE_BINDER
	if (m_shaderProgram != program)
#endif
	{
		m_shaderProgram = program;
		glUseProgram(m_shaderProgram);
	}
}
//-----------------------------------------------------------------------------
void OpenGLStateCache::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
#if SE_OGL_CASHE_BINDER
	if (m_viewportSize[0] != x || m_viewportSize[1] != y || m_viewportSize[2] != width || m_viewportSize[3] != height)
#endif
	{
		m_viewportSize[0] = x;
		m_viewportSize[1] = y;
		m_viewportSize[2] = width;
		m_viewportSize[3] = height;
		glViewport(x, y, width, height);
	}
}
//-----------------------------------------------------------------------------