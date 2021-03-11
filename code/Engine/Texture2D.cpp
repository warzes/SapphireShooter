#include "stdafx.h"
#if SE_OPENGL
#include "Texture2D.h"
#include "OGLFunc.h"
//-----------------------------------------------------------------------------
void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data, bool alpha, GLboolean mipmaps)
{
	Width = width;
	Height = height;

	if (alpha)
	{
		InternalFormat = GL_RGBA;
		ImageFormat = GL_RGBA;
	}
	else
	{
		InternalFormat = GL_RGB;
		ImageFormat = GL_RGB;
	}

	// Create Texture
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, ImageFormat, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMax);
	// Mipmap generation
	if (mipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}
//-----------------------------------------------------------------------------
void Texture2D::Bind(unsigned unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}
//-----------------------------------------------------------------------------
void Texture2D::Delete()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &ID);
}
//-----------------------------------------------------------------------------
#endif