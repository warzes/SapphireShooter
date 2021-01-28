#include "stdafx.h"
#include "ShaderProgram.h"
#include "OGLFunc.h"
#include "OpenGLStateCache.h"
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	// vertex Shader
	unsigned int sVertex = createShader(GL_VERTEX_SHADER, vertexSource, "VERTEX");
	// fragment Shader
	unsigned int sFragment = createShader(GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT");
	// if geometry shader source code is given, also compile geometry shader
	unsigned int sGeometry = 0;
	if (geometrySource != nullptr)
		sGeometry = createShader(GL_GEOMETRY_SHADER, geometrySource, "GEOMETRY");

	// shader program	
	program = glCreateProgram();
	glAttachShader(program, sVertex);
	glAttachShader(program, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(program, sGeometry);
	glLinkProgram(program);
	checkProgramCompileErrors(program);
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(sGeometry);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	OpenGLStateCache::Get().UseProgram(program);
}
//-----------------------------------------------------------------------------
void ShaderProgram::UnBind() const
{
	OpenGLStateCache::Get().UseProgram(0);
}
//-----------------------------------------------------------------------------
void ShaderProgram::Destroy()
{
	glDeleteProgram(program);
	program = 0;
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetBool(const char* name, bool value)
{
	Bind();
	glUniform1i(glGetUniformLocation(program, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetFloat(const char* name, float value)
{
	Bind();
	glUniform1f(glGetUniformLocation(program, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetInteger(const char* name, int value)
{
	Bind();
	glUniform1i(glGetUniformLocation(program, name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, float x, float y)
{
	Bind();
	glUniform2f(glGetUniformLocation(program, name), x, y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, const glm::vec2& value)
{
	Bind();
	glUniform2f(glGetUniformLocation(program, name), value.x, value.y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, float x, float y, float z)
{
	Bind();
	glUniform3f(glGetUniformLocation(program, name), x, y, z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, const glm::vec3& value)
{
	Bind();
	glUniform3f(glGetUniformLocation(program, name), value.x, value.y, value.z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, float x, float y, float z, float w)
{
	Bind();
	glUniform4f(glGetUniformLocation(program, name), x, y, z, w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, const glm::vec4& value)
{
	Bind();
	glUniform4f(glGetUniformLocation(program, name), value.x, value.y, value.z, value.w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, false, glm::value_ptr(matrix));
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