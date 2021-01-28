#pragma once

/* класс-утилита кеширующий состояния OpenGL*/

class OpenGLStateCache
{
public:
	static OpenGLStateCache& Get()
	{
		static OpenGLStateCache instance;
		return instance;
	}

	void UseProgram(GLuint program);

	void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

private:
	GLuint m_shaderProgram = 0;
	int m_viewportSize[4] = {0};
};