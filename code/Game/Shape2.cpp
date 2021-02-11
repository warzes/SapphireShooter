#include "stdafx.h"
#include "Shape2.h"

Shape2::Shape2()
{
	amount = new unsigned;
	*amount = 1;
}

Shape2::Shape2(const Shape2& shape)
{
	swap(shape);
}

Shape2& Shape2::operator=(const Shape2& shape)
{
	clear();
	swap(shape);
	return *this;
}

void Shape2::swap(const Shape2& shape)
{
	VAO = shape.VAO;
	VBO = shape.VBO;
	amount = shape.amount;
	*amount = *amount + 1;
}

void Shape2::clear()
{
	*amount = *amount - 1;
	if (*amount == 0)
	{
		delete amount;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

Shape2::~Shape2()
{
	clear();
}