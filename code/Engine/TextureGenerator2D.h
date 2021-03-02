#pragma once

#include "TextureGenerator.h"

class TextureGenerator2D : public TextureGenerator
{
public:
	virtual void generate(Texture& texture, const TextureGenerateMethod& method, const unsigned& width, const unsigned& height, const unsigned& depth = 1);
};