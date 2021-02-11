#pragma once

#include "Texture.h"

class ConverterToNormalMap
{
	GLubyte toRGB(const float& pixel);
	int clamp(const int& value, const int& max);
	void prepareData(GLubyte* inputData, const std::vector<GLubyte>& data, const unsigned& width, const unsigned& height);
	glm::vec3 calcNormal(const std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, const int& row, const int& column);
	float getHeight(const std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, int row, int column);

public:
	void convert(const std::vector<GLubyte>& data, Texture& textureDestination, const unsigned& width, const unsigned& height);
};