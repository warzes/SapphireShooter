#include "stdafx.h"
#include "TextureManager.h"
#include "OGLFunc.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION
//-----------------------------------------------------------------------------
std::shared_ptr<Texture2D> TextureManager::LoadTexture(const std::string& name, const char* fileTexture, bool alpha)
{
	// first check if texture has been loader already, if so; return earlier loaded texture
	auto it = m_textures.find(name);
	if (it != m_textures.end())
		return it->second;

	std::shared_ptr<Texture2D> texture(new Texture2D);
	// Load image
	loadTextureFromFile(texture, fileTexture, alpha);
	// Store and return
	m_textures[name] = texture;
	return texture;
}
//-----------------------------------------------------------------------------
std::shared_ptr<Texture2D> TextureManager::GetTexture(const std::string& name)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
		return it->second;
	else
		return std::shared_ptr<Texture2D>();
}
//-----------------------------------------------------------------------------
void TextureManager::Clear()
{
	//for (auto iter : m_textures)
	//	iter.second->Delete();
	//m_textures.clear();
}
//-----------------------------------------------------------------------------
void TextureManager::loadTextureFromFile(std::shared_ptr<Texture2D> outTexture, const char* file, bool alpha)
{
	// load image
	int desiredChannels = 0;
	if (alpha)
		desiredChannels = STBI_rgb_alpha;
	else
		desiredChannels = STBI_rgb;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, desiredChannels);
	// now generate texture
	outTexture->Generate(width, height, data, alpha);
	// and finally free image data
	stbi_image_free(data);
}
//-----------------------------------------------------------------------------