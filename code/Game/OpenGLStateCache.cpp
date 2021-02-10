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