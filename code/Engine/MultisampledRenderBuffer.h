#pragma once

#if SE_OPENGL
#include "RenderBuffer.h"

class MultisampledRenderBuffer : public RenderBuffer
{
public:
	virtual void create(const unsigned& width, const unsigned& height);
};
#endif