#pragma once

#include "TextureGenerator.h"

class PerlinNoise2D : public TextureGenerateMethod
{
	double frequency;
	double amplitude;
	double persistence;
	double multiplier;
	unsigned offsetX;
	unsigned offsetY;
	unsigned octaves;

	double octavePerlin(const double& x, const double& y) const;

public:
	PerlinNoise2D(const double& pFrequency = 1, const double& pAmplitude = 1, const double& pPersistance = 1, const unsigned& pOctaves = 4, const double& pMulitplier = 1, const unsigned& pOffsetX = 0, const unsigned& pOffsetY = 0);
	virtual void fillData(std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, const unsigned& depth) const;
};