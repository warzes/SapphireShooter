#pragma once

#include "Texture2D.h"

class TextureManager
{
public:
	// retrieves a single instance of this object
	static TextureManager& Get() noexcept
	{
		static TextureManager theInstance;
		return theInstance;
	}

	// loads a texture from file with a given name
	std::shared_ptr<Texture2D> LoadTexture(const std::string& name, const char* fileTexture, bool alpha);
	// retrieves a texture given its name if previously loaded 
	std::shared_ptr<Texture2D> GetTexture(const std::string& name);

	void Clear();

private:
	TextureManager() = default;

	// loads a single texture from file
	void loadTextureFromFile(std::shared_ptr<Texture2D> outTexture, const char* file, bool alpha);

	std::map<std::string, std::shared_ptr<Texture2D>> m_textures;
};