#pragma once

#if SE_OPENGL

#include "Singleton.h"
#include "ShaderProgram.h"

class ShaderManager final : public Singleton<ShaderManager>
{
public:
	std::shared_ptr<ShaderProgram> LoadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	std::shared_ptr<ShaderProgram> GetShader(const std::string &name);
	
	void Clear();

private:
	std::map<std::string, std::shared_ptr<ShaderProgram>> m_shaders;
};
#endif