#include "stdafx.h"
#if SE_OPENGL
#include "ShaderManager.h"
#include "OGLFunc.h"
#include "Log.h"
//-----------------------------------------------------------------------------
std::shared_ptr<ShaderProgram> ShaderManager::LoadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return it->second;

	std::shared_ptr<ShaderProgram> shader(new ShaderProgram);
	shader->CompileFromFile(vShaderFile, fShaderFile, gShaderFile);
	m_shaders[name] = shader;
	return shader;
}
//-----------------------------------------------------------------------------
std::shared_ptr<ShaderProgram> ShaderManager::GetShader(const std::string& name)
{
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return it->second;
	else
		return std::shared_ptr<ShaderProgram>();
}
//-----------------------------------------------------------------------------
void ShaderManager::Clear()
{
	for (auto iter : m_shaders)
		iter.second->Destroy();
	m_shaders.clear();
}
//-----------------------------------------------------------------------------
#endif