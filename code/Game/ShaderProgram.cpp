#include "stdafx.h"
#include "ShaderProgram.h"
#include "OGLFunc.h"
#include "OpenGLStateCache.h"
//-----------------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	OpenGLStateCache::Get().UseProgram(ID);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	// vertex Shader
	unsigned int sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, nullptr);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// fragment Shader
	unsigned int sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, nullptr);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// if geometry shader source code is given, also compile geometry shader
	unsigned int gShader;
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, nullptr);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// shader program	
	ID = glCreateProgram();
	glAttachShader(ID, sVertex);
	glAttachShader(ID, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(ID, gShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Destroy()
{
	// TODO:
	glDeleteProgram(ID);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetFloat(const char* name, float value)
{
	Bind();
	glUniform1f(glGetUniformLocation(ID, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetInteger(const char* name, int value)
{
	Bind();
	glUniform1i(glGetUniformLocation(ID, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, float x, float y)
{
	Bind();
	glUniform2f(glGetUniformLocation(ID, name), x, y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, const glm::vec2& value)
{
	Bind();
	glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, float x, float y, float z)
{
	Bind();
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, const glm::vec3& value)
{
	Bind();
	glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, float x, float y, float z, float w)
{
	Bind();
	glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, const glm::vec4& value)
{
	Bind();
	glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}
//-----------------------------------------------------------------------------
void ShaderProgram::checkCompileErrors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}
//-----------------------------------------------------------------------------