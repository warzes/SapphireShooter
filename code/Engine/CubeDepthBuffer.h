#pragma once

#if SE_OPENGL

#include "DepthBuffer.h"

class CubeDepthBuffer : public DepthBuffer
{
public:
	virtual void create(const unsigned& width, const unsigned& height);
	virtual void attach() const;
};

#endif