#include "stdafx.h"
#include "ShaderProgram.h"
#include "OGLFunc.h"
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(Shader& vertex, Shader& fragment)
{
	Create();
	AttachShader(vertex);
	AttachShader(fragment);
	Link();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(Shader& vertex, Shader& fragment, Shader& geometry)
{
	Create();
	AttachShader(vertex);
	AttachShader(fragment);
	AttachShader(geometry);
	Link();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	Create();
	Shader vertex(ShaderType::Vertex, vertexSource, false);
	Shader fragment(ShaderType::Fragment, fragmentSource, false);
	Shader geometry;
	if (geometrySource != nullptr)
		geometry.CreateFromMemory(ShaderType::Geometry, geometrySource);
	AttachShader(vertex);
	AttachShader(fragment);
	if (geometrySource != nullptr)
		AttachShader(geometry);
	Link();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Create()
{
	if (m_programId != 0)
		throw std::runtime_error("Can't create exists program");
	m_programId = glCreateProgram();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Link() const
{
	if (m_programId == 0)
		throw std::runtime_error("Can't link empty program");
	glLinkProgram(m_programId);
	int success;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success)
		throw std::runtime_error(getLinkMessageErrorAndClear());
}
//-----------------------------------------------------------------------------
void ShaderProgram::AttachShader(Shader& shader) const
{
	if (m_programId == 0)
		throw std::runtime_error("Can't attach shader to empty program");
	glAttachShader(m_programId, shader.GetId());
	shader.Destroy();
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
	glUniform1i(getUniformId(name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetFloat(const char* name, float value)
{
	Bind();
	glUniform1f(getUniformId(name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetInteger(const char* name, int value)
{
	Bind();
	glUniform1i(getUniformId(name), value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, float x, float y)
{
	Bind();
	glUniform2f(getUniformId(name), x, y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector2f(const char* name, const glm::vec2& value)
{
	Bind();
	glUniform2f(getUniformId(name), value.x, value.y);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, float x, float y, float z)
{
	Bind();
	glUniform3f(getUniformId(name), x, y, z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector3f(const char* name, const glm::vec3& value)
{
	Bind();
	glUniform3f(getUniformId(name), value.x, value.y, value.z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, float x, float y, float z, float w)
{
	Bind();
	glUniform4f(getUniformId(name), x, y, z, w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetVector4f(const char* name, const glm::vec4& value)
{
	Bind();
	glUniform4f(getUniformId(name), value.x, value.y, value.z, value.w);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	Bind();
	glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
//-----------------------------------------------------------------------------
std::string ShaderProgram::getLinkMessageErrorAndClear() const
{
	int length;
	glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &length);
	char* message = new char[length];

	glGetProgramInfoLog(m_programId, length, &length, message);
	glDeleteProgram(m_programId);

	std::string finalMess = message;
	delete[] message;
	return finalMess;
}
//-----------------------------------------------------------------------------
unsigned ShaderProgram::getUniformId(const char* name) const
{
	return glGetUniformLocation(m_programId, name);
}
//-----------------------------------------------------------------------------