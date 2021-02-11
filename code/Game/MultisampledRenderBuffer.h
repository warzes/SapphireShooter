#pragma once

#include "RenderBuffer.h"

class MultisampledRenderBuffer : public RenderBuffer
{
public:
	virtual void create(const unsigned& width, const unsigned& height);
};