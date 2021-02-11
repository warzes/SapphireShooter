#include "stdafx.h"
#include "ShaderProgram.h"
#include "OGLFunc.h"
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
#error переделать. Возможно отказаться от ShaderManager


	// vertex Shader
	unsigned int sVertex = createShader(GL_VERTEX_SHADER, vertexSource, "VERTEX");
	// fragment Shader
	unsigned int sFragment = createShader(GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT");
	// if geometry shader source code is given, also compile geometry shader
	unsigned int sGeometry = 0;
	if (geometrySource != nullptr)
		sGeometry = createShader(GL_GEOMETRY_SHADER, geometrySource, "GEOMETRY");

	// shader program	
	m_programId = glCreateProgram();
	glAttachShader(m_programId, sVertex);
	glAttachShader(m_programId, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(m_programId, sGeometry);
	glLinkProgram(m_programId);
	checkProgramCompileErrors(m_programId);
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(sGeometry);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	glUseProgram(m_programId);
}
//-----------------------------------------------------------------------------
void ShaderProgram::UnBind() const
{
	glUseProgram(0);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Destroy()
{
	glDeleteProgram(m_programId);
	m_programId = 0;
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetBool(const char* name, bool value)
{
	Bind();
	glUniform1i(glGetUniformLocation(m_programId, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetFloat(const char* name, float value)
{
	Bind();
	glUniform1f(glGetUniformLocation(m_programId, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetInteger(const char* name, int value)
{
	Bind();
	glUniform1i(glGetUniformLocation(m_programId, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, float x, float y)
{
	Bind();
	glUniform2f(glGetUniformLocation(m_programId, name), x, y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, const glm::vec2& value)
{
	Bind();
	glUniform2f(glGetUniformLocation(m_programId, name), value.x, value.y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, float x, float y, float z)
{
	Bind();
	glUniform3f(glGetUniformLocation(m_programId, name), x, y, z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, const glm::vec3& value)
{
	Bind();
	glUniform3f(glGetUniformLocation(m_programId, name), value.x, value.y, value.z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, float x, float y, float z, float w)
{
	Bind();
	glUniform4f(glGetUniformLocation(m_programId, name), x, y, z, w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, const glm::vec4& value)
{
	Bind();
	glUniform4f(glGetUniformLocation(m_programId, name), value.x, value.y, value.z, value.w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
//-----------------------------------------------------------------------------
GLuint ShaderProgram::createShader(GLenum shaderType, const char* source, const std::string& typeName)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	checkShaderErrors(shader, typeName);
	return shader;
}
//-----------------------------------------------------------------------------
void ShaderProgram::checkShaderErrors(unsigned int object, const std::string& type)
{
	int success;
	char infoLog[1024];
	glGetShaderiv(object, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(object, 512, NULL, infoLog);
		std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
			<< infoLog << "\n -- --------------------------------------------------- -- "
			<< std::endl;
	}
}
//-----------------------------------------------------------------------------
void ShaderProgram::checkProgramCompileErrors(unsigned int object)
{
	int success;
	char infoLog[1024];
	glGetProgramiv(object, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(object, 512, NULL, infoLog);
		std::cout << "| ERROR::Shader: Link-time error: Type: PROGRAM\n"
			<< infoLog << "\n -- --------------------------------------------------- -- "
			<< std::endl;
	}
}
//-----------------------------------------------------------------------------