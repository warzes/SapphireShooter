#pragma once

#include "TextureGenerator.h"

class Random : public TextureGenerateMethod
{
	unsigned min;
	unsigned max;

public:
	Random(const unsigned& pMin, const unsigned& pMax) :min(pMin), max(pMax) { ; }
	virtual void fillData(std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, const unsigned& depth) const;
};