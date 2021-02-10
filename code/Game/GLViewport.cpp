#include "stdafx.h"
#include "GLViewport.h"
//-----------------------------------------------------------------------------
GLint GLViewport::ms_positionX = 0;
GLint GLViewport::ms_positionY = 0;
GLsizei GLViewport::ms_width = 0;
GLsizei GLViewport::ms_height = 0;
float GLViewport::ms_clearColorR = 0.f;
float GLViewport::ms_clearColorG = 0.f;
float GLViewport::ms_clearColorB = 0.f;
float GLViewport::ms_clearColorA = 0.f;
GLenum GLViewport::ms_blendSource = GL_ONE;
GLenum GLViewport::ms_blendDestination = GL_ZERO;
float GLViewport::ms_lineWidth = 1.f;
//-----------------------------------------------------------------------------
void GLViewport::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	bool update = false;
	if (ms_positionX != x)
	{
		ms_positionX = x;
		update = true;
	}
	if (ms_positionY != y)
	{
		ms_positionY = y;
		update = true;
	}
	if (ms_width != width)
	{
		ms_width = width;
		update = true;
	}
	if (ms_height != height)
	{
		ms_height = height;
		update = true;
	}

#if SE_OPENGL_CASHE
	if (update) 
#endif
		glViewport(ms_positionX, ms_positionY, ms_width, ms_height);
}
//-----------------------------------------------------------------------------
void GLViewport::Clear(bool depth, bool color)
{
	GLenum mode = 0;
	if (depth) mode |= GL_DEPTH_BUFFER_BIT;
	if (color) mode |= GL_COLOR_BUFFER_BIT;
	if (mode) glClear(mode);
}
//-----------------------------------------------------------------------------
void GLViewport::SetClearColor(float red, float green, float blue, float alpha)
{
	bool update = false;
	if (ms_clearColorR != red)
	{
		ms_clearColorR = red;
		update = true;
	}
	if (ms_clearColorG != green)
	{
		ms_clearColorG = green;
		update = true;
	}
	if (ms_clearColorB != blue)
	{
		ms_clearColorB = blue;
		update = true;
	}
	if (ms_clearColorA != alpha)
	{
		ms_clearColorA = alpha;
		update = true;
	}
#if SE_OPENGL_CASHE
	if (update) 
#endif
		glClearColor(ms_clearColorR, ms_clearColorG, ms_clearColorB, ms_clearColorA);
}
//-----------------------------------------------------------------------------
void GLViewport::SetBlendFunctions(GLenum src, GLenum dst)
{
	bool update = false;
	if (ms_blendSource != src)
	{
		ms_blendSource = src;
		update = true;
	}
	if (ms_blendDestination != dst)
	{
		ms_blendDestination = dst;
		update = true;
	}
#if SE_OPENGL_CASHE
	if (update) 
#endif
		glBlendFunc(ms_blendSource, ms_blendDestination);
}
//-----------------------------------------------------------------------------
void GLViewport::SetLineWidth(float width)
{
#if SE_OPENGL_CASHE
	if (ms_lineWidth != width)
#endif
	{
		ms_lineWidth = width;
		glLineWidth(ms_lineWidth);
	}
}
//-----------------------------------------------------------------------------