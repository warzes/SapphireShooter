#pragma once

class Texture2D
{
public:
	void Generate(unsigned int width, unsigned int height, unsigned char* data, bool alpha = true, GLboolean mipmaps = GL_TRUE);
	void Bind(unsigned unit) const;
	void Delete();

	unsigned int ID = 0;
	// texture image dimensions
	unsigned int Width = 0;               // width of loaded image in pixels
	unsigned int Height = 0;              // height of loaded image in pixels

	// TODO: код ниже возможно не нужно сохранять
	// texture Format
	unsigned int InternalFormat = GL_RGBA;// format of texture object
	unsigned int ImageFormat = GL_RGBA;   // format of loaded image
	// texture configuration
	unsigned int WrapS = GL_REPEAT;       // wrapping mode on S axis
	unsigned int WrapT = GL_REPEAT;       // wrapping mode on T axis
	unsigned int FilterMin = GL_LINEAR_MIPMAP_NEAREST;// filtering mode if texture pixels < screen pixels
	unsigned int FilterMax = GL_NEAREST;  // filtering mode if texture pixels > screen pixels
};