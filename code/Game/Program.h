#pragma once

#include "Shader.h"

// todo: rename ShaderProgram

class Program
{
public:
	Program() = default;
	Program(const Shader& vertex, const Shader& fragment);
	Program(const std::string& vertFileName, const std::string& fragFileName);
	~Program();

	void Create();
	void Link() const;
	void AttachShader(const Shader& shader) const;

	void Bind() const { glUseProgram(m_programId); }
	//void UnBind() const { glUseProgram(0); }

	void SetInt(const char* name, int i) const;
	void SetFloat(const char* name, float f) const;
	void SetVec2(const char* name, const glm::vec2& vec) const;
	void SetVec3(const char* name, const glm::vec3& vec) const;
	void SetVec4(const char* name, const glm::vec4& vec) const;
	void SetMat2(const char* name, const glm::mat2& mat) const;
	void SetMat3(const char* name, const glm::mat3& mat) const;
	void SetMat4(const char* name, const glm::mat4& mat) const;

	unsigned GetId() const { return m_programId; }

private:
	std::string getLinkMessageErrorAndClear() const;
	unsigned getUniformId(const char* name) const;

	unsigned m_programId = 0;
};