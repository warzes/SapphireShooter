#pragma once

#include "Shader.h"

class ShaderProgram
{
	friend class ShaderManager;
public:
	void Compile(Shader& vertex, Shader& fragment);
	void Compile(Shader& vertex, Shader& fragment, Shader& geometry);
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	void Destroy();

	void Create();
	void Link() const;
	void AttachShader(Shader& shader) const;

	void Bind() const;
	void UnBind() const;
	
	void SetBool(const char* name, bool value);
	void SetFloat(const char* name, float value);
	void SetInteger(const char* name, int value);
	void SetVector2f(const char* name, float x, float y);
	void SetVector2f(const char* name, const glm::vec2& value);
	void SetVector3f(const char* name, float x, float y, float z);
	void SetVector3f(const char* name, const glm::vec3& value);
	void SetVector4f(const char* name, float x, float y, float z, float w);
	void SetVector4f(const char* name, const glm::vec4& value);
	void SetMatrix4(const char* name, const glm::mat4& matrix);

	unsigned GetId() const { return m_programId; }
		
private:
	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&&) = delete;
	void operator=(const ShaderProgram&) = delete;
	void operator=(ShaderProgram&&) = delete;

	std::string getLinkMessageErrorAndClear() const;
	unsigned getUniformId(const char* name) const;

	unsigned m_programId = 0u;
};