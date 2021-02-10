#include "stdafx.h"
#include "Program.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
Program::Program()
{
	m_amount = new unsigned;
	*m_amount = 1;
}
//-----------------------------------------------------------------------------
Program::Program(const Shader& vertex, const Shader& fragment)
{
	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertex.GetId());
	glAttachShader(m_programId, fragment.GetId());
	Link();
	vertex.Clear();
	fragment.Clear();
	m_amount = new unsigned;
	*m_amount = 1;
}
//-----------------------------------------------------------------------------
Program::Program(const std::string& vertFileName, const std::string& fragFileName)
{
	m_programId = glCreateProgram();
	Shader vertex = Shader::CreateVertexShader(vertFileName);
	Shader fragment = Shader::CreateFragmentShader(fragFileName);
	glAttachShader(m_programId, vertex.GetId());
	glAttachShader(m_programId, fragment.GetId());
	Link();
	vertex.Clear();
	fragment.Clear();
	m_amount = new unsigned;
	*m_amount = 1;
}
//-----------------------------------------------------------------------------
Program::Program(const Program& program)
{
	swap(program);
}
//-----------------------------------------------------------------------------
Program& Program::operator=(const Program& program)
{
	clear();
	swap(program);
	return *this;
}
//-----------------------------------------------------------------------------
void Program::swap(const Program& program)
{
	m_programId = program.m_programId;
	m_amount = program.m_amount;
	*m_amount = *m_amount + 1;
}
//-----------------------------------------------------------------------------
void Program::clear()
{
	*m_amount = *m_amount - 1;
	if (*m_amount == 0)
	{
		delete m_amount;
		if (m_programId != 0)
			glDeleteProgram(m_programId);
	}
}
//-----------------------------------------------------------------------------
Program::~Program()
{
	clear();
}
//-----------------------------------------------------------------------------
std::string Program::getLinkMessageErrorAndClear() const
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
void Program::Create()
{
	if (m_programId != 0)
		throw std::runtime_error("Can't create exists program");
	m_programId = glCreateProgram();
}
//-----------------------------------------------------------------------------
void Program::AttachShader(const Shader& shader) const
{
	if (m_programId == 0)
		throw std::runtime_error("Can't attach shader to empty program");
	glAttachShader(m_programId, shader.GetId());
	shader.Clear();
}
//-----------------------------------------------------------------------------
void Program::Link() const
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
unsigned Program::getUniformId(const char* name) const
{
	return glGetUniformLocation(m_programId, name);
}
//-----------------------------------------------------------------------------
void Program::SetInt(const char* name, int i) const
{
	glUniform1i(getUniformId(name), i);
}
//-----------------------------------------------------------------------------
void Program::SetFloat(const char* name, float f) const
{
	glUniform1f(getUniformId(name), f);
}
//-----------------------------------------------------------------------------
void Program::SetVec2(const char* name, const glm::vec2& vec) const
{
	glUniform2fv(getUniformId(name), 1, glm::value_ptr(vec));
}
//-----------------------------------------------------------------------------
void Program::SetVec3(const char* name, const glm::vec3& vec) const
{
	glUniform3fv(getUniformId(name), 1, glm::value_ptr(vec));
}
//-----------------------------------------------------------------------------
void Program::SetVec4(const char* name, const glm::vec4& vec) const
{
	glUniform4fv(getUniformId(name), 1, glm::value_ptr(vec));
}
//-----------------------------------------------------------------------------
void Program::SetMat2(const char* name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}
//-----------------------------------------------------------------------------
void Program::SetMat3(const char* name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}
//-----------------------------------------------------------------------------
void Program::SetMat4(const char* name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}
//-----------------------------------------------------------------------------