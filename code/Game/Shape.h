#pragma once

#include "Vertex.h"

enum class ShapeType
{
	Triangle,
	Quad,
	Cube,
	Sphere
};

class Shape
{
public:
	virtual ~Shape() = default;

	static Shape* CreateShape(ShapeType shapetype);

	virtual std::vector<Vertex> GetVertexData() = 0;
	virtual unsigned int GetVertexDataCount() = 0;
	virtual unsigned int* GetIndexData() = 0;
	virtual unsigned int GetIndexDataCount() = 0;
	virtual void InitVertexData() = 0;
};