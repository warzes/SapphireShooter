#include "stdafx.h"
#include "ShaderManager.h"
#include "OGLFunc.h"
#include "Log.h"
//-----------------------------------------------------------------------------
std::shared_ptr<ShaderProgram> ShaderManager::LoadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// first check if shader has been loader already, if so; return earlier loaded texture
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return it->second;
	std::shared_ptr<ShaderProgram> shader(new ShaderProgram);
	loadShaderFromFile(shader, vShaderFile, fShaderFile, gShaderFile);

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
	//for (auto iter : m_shaders)
	//	iter.second->Destroy();
	//m_shaders.clear();
}
//-----------------------------------------------------------------------------
void ShaderManager::loadShaderFromFile(std::shared_ptr<ShaderProgram> outShaders, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::ifstream geometryShaderFile;
	// ensures ifstream objects can throw exceptions:
	vertexShaderFile.exceptions(std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// open files
		vertexShaderFile.open(vShaderFile, std::ios::in);
		if (!vertexShaderFile.good())
		{
			std::cout << "SHADER ERROR: Unable to load shader: " << vShaderFile << "\n";
		}
		fragmentShaderFile.open(fShaderFile, std::ios::in);
		if (!fragmentShaderFile.good())
		{
			std::cout << "SHADER ERROR: Unable to load shader: " << fShaderFile << "\n";
		}
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			geometryShaderFile.open(gShaderFile, std::ios::in);
			if (!geometryShaderFile.good())
			{
				std::cout << "SHADER ERROR: Unable to load shader: " << gShaderFile << "\n";
			}
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	catch (std::exception e)
	{
		// TODO:
	}

#if SE_LOG_SHADER_CODE
	SE_LOG("VERTEX SHADER CODE:");
	SE_LOG(vertexCode);
	SE_LOG("FRAGMENT SHADER CODE:");
	SE_LOG(fragmentCode);
	if (gShaderFile != nullptr)
	{
		SE_LOG("GEOMETRY SHADER CODE:");
		SE_LOG(geometryCode);
	}
#endif

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	outShaders->Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
}
//-----------------------------------------------------------------------------