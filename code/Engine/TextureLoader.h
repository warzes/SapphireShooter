#pragma once

class TextureLoader
{
public:
	static void LoadTexture(unsigned& textureId, const std::string& fileName, GLenum type);
	static void LoadTexture2D(unsigned& textureId, const std::string& fileName);
	static void LoadCubeMapTexture(unsigned& textureId, const std::vector<std::string>& fileNames);
private:
	static std::string getError(const std::string& fileName);
	static GLenum getFormat(const unsigned& nrChannels);
};