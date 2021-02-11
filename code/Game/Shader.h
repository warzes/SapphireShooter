#pragma once

#include "OGLFunc.h"

enum class ShaderType
{
	Vertex,
	Fragment,
	Geometry,
	TessalationControl,
	TessalationEvaluation, 
	Compute
};

class Shader
{
public:
	static Shader CreateVertexShader(const std::string& fileName)
	{
		return Shader(fileName, ShaderType::Vertex);
	}

	static Shader CreateFragmentShader(const std::string& fileName)
	{
		return Shader(fileName, ShaderType::Fragment);
	}

	static Shader CreateGeometryShader(const std::string& fileName)
	{
		return Shader(fileName, ShaderType::Geometry);
	}

	static Shader CreateTessalationControlShader(const std::string& fileName)
	{
		return Shader(fileName, ShaderType::TessalationControl);
	}

	static Shader CreateTessalationEvaluationShader(const std::string& fileName)
	{
		return Shader(fileName, ShaderType::TessalationEvaluation);
	}

	//static Shader CreateComputeShader(const std::string& fileName)
	//{
	//	return Shader(fileName, ShaderType::Compute);
	//}

	void Clear() const { glDeleteShader(m_shaderId);  }

	unsigned GetId() const { return m_shaderId; }
	GLenum GetType() const { return m_type; }
private:
	Shader(const std::string& fileName, ShaderType type);
	std::string getSource(const std::string& fileName) const;
	std::string getCompileMessageErrorAndClear() const;

	unsigned m_shaderId;
	GLenum m_type;	
};