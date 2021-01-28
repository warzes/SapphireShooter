#pragma once

class ShaderProgram
{
public:
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	void Destroy();

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

	unsigned int program = 0;
private:
	GLuint createShader(GLenum shaderType, const char* source, const std::string& typeName);
	void checkShaderErrors(unsigned int object, const std::string& type);
	void checkProgramCompileErrors(unsigned int object);
};