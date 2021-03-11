#include "stdafx.h"
#if SE_OPENGL
#include "Random.h"

void Random::fillData(std::vector<GLubyte>& data, const unsigned& width, const unsigned& height, const unsigned& depth) const
{
	const unsigned SIZE = width * height * depth * 4;
	for (unsigned i = 0; i < SIZE; ++i)
		data[i] = (GLubyte)(rand() % max) + min;
}
#endif