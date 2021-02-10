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

private:
	GLuint m_shaderProgram = 0;
};