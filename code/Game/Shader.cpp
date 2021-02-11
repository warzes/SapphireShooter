#include "stdafx.h"
#include "Shader.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr inline GLenum translateShaderType(ShaderType type)
{
	switch (type)
	{
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
	case ShaderType::TessalationControl: return GL_TESS_CONTROL_SHADER;
	case ShaderType::TessalationEvaluation: return GL_TESS_EVALUATION_SHADER;
	//case ShaderType::Compute: return GL_COMPUTE_SHADER;
	}

	return 0;
}
//-----------------------------------------------------------------------------
Shader::Shader(const std::string& fileName, ShaderType type) 
	: m_type(translateShaderType(type))
{
	std::string source = getSource(fileName);
	const char* data = source.c_str();
	m_shaderId = glCreateShader(m_type);
	glShaderSource(m_shaderId, 1, &data, nullptr);
	glCompileShader(m_shaderId);

	int success;
	glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
		throw std::runtime_error(getCompileMessageErrorAndClear());
}
//-----------------------------------------------------------------------------
std::string Shader::getSource(const std::string& fileName) const
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Can\'t open shader file: " + fileName + ".");

	std::stringstream stream;
	stream << file.rdbuf();

	file.clear();
	file.close();
	return stream.str();
}
//-----------------------------------------------------------------------------
std::string Shader::getCompileMessageErrorAndClear() const
{
	int length;
	glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &length);
	char* message = new char[length];

	glGetShaderInfoLog(m_shaderId, length, &length, message);
	glDeleteShader(m_shaderId);

	std::string finalMess = message;
	delete[] message;
	return finalMess;
}
//-----------------------------------------------------------------------------