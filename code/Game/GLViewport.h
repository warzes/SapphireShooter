#pragma once

class GLViewport final
{
public:
	static void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	static void Clear(bool depth, bool color);
	static void SetClearColor(float red, float green, float blue, float alpha);
	static void SetBlendFunctions(GLenum src, GLenum dst);
	static void SetLineWidth(float width);

private:
	GLViewport() = delete;
	GLViewport(const GLViewport&) = delete;
	GLViewport& operator=(const GLViewport&) = delete;
	~GLViewport() = delete;

	SE_THREADTYPE static GLint ms_positionX;
	SE_THREADTYPE static GLint ms_positionY;
	SE_THREADTYPE static GLsizei ms_width;
	SE_THREADTYPE static GLsizei ms_height;
	SE_THREADTYPE static float ms_clearColorR;
	SE_THREADTYPE static float ms_clearColorG;
	SE_THREADTYPE static float ms_clearColorB;
	SE_THREADTYPE static float ms_clearColorA;
	SE_THREADTYPE static GLenum ms_blendSource;
	SE_THREADTYPE static GLenum ms_blendDestination;
	SE_THREADTYPE static float ms_lineWidth;
};