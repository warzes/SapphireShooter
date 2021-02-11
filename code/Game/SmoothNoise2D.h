#pragma once

#include "TextureGenerator.h"

typedef std::vector<std::vector<double> > NoiseData2D;
class SmoothNoise2D : public TextureGenerateMethod
{
	unsigned zoom;

	double turbulence(const NoiseData2D& pattern, const unsigned& width, const unsigned& height, double x, double y) const;
	double smooth(const NoiseData2D& pattern, const unsigned& width, const unsigned& height, double x1, double y1) const;

public:
	SmoothNoise2D(const unsigned& zoomLevel = 1) : zoom(zoomLevel) { if (zoom == 0) zoom = 1; }
	virtual void fillData(std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, const unsigned& depth) const;
};