#pragma once

#include "Renderable.h"

class Shape2 : public Renderable
{
	void swap(const Shape2& shape);
	void clear();

protected:
	unsigned* amount;
	unsigned VAO = 0;
	unsigned VBO = 0;

	bool willBeClear() const { return (*amount - 1) == 0; }

public:
	Shape2();
	Shape2(const Shape2& shape);
	Shape2& operator=(const Shape2& shape);
	virtual ~Shape2();

	virtual unsigned GetVAO() const { return VAO; }
};