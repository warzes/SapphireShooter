#pragma once

#if SE_OPENGL

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

// TODO: возможно шейдер должен грузится только с памяти, загрузка из файла в менеджере

class Shader
{
public:
	Shader() = default;
	Shader(ShaderType type, const std::string& source, bool fromFile = true);
	~Shader();

	bool CreateFromFile(ShaderType type, const std::string& fileName);
	bool CreateFromMemory(ShaderType type, const std::string& memory);
	
	void Destroy() { if (m_shaderId) glDeleteShader(m_shaderId); m_shaderId = 0u; }

	unsigned GetId() const { return m_shaderId; }
private:
	std::string getSourceFromFile(const std::string& fileName) const;
	std::string getCompileMessageErrorAndClear() const;
	bool loadShaderCode(ShaderType type, const char* data);

	unsigned m_shaderId = 0;
};

#endif