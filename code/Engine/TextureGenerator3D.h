#pragma once

#if SE_OPENGL
#include "TextureGenerator.h"

class TextureGenerator3D : public TextureGenerator
{
public:
	virtual void generate(Texture& texture, const TextureGenerateMethod& method, const unsigned& width, const unsigned& height, const unsigned& depth);
};

#endif