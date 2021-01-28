#pragma once

#include "ShaderProgram.h"

class ShaderManager
{
public:
	// retrieves a single instance of this object
	static ShaderManager& Get() noexcept
	{
		static ShaderManager theInstance;
		return theInstance;
	}

	// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	std::shared_ptr<ShaderProgram> LoadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	// retrieves a stored sader
	std::shared_ptr<ShaderProgram> GetShader(const std::string &name);
	
	void Clear();

private:
	ShaderManager() = default;
	// loads and generates a shader from file
	void loadShaderFromFile(std::shared_ptr<ShaderProgram> outShaders, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	std::map<std::string, std::shared_ptr<ShaderProgram>> m_shaders;
};