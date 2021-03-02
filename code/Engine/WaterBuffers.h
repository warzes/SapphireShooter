#pragma once

#include "FrameBuffer2.h"
#include "RenderBuffer.h"

class WaterBuffers
{
	FrameBuffer reflectionFrame;
	FrameBuffer refractionFrame;
	RenderBuffer reflectRenderBuffer;
	RenderBuffer refractRenderBuffer;
	DepthBuffer refractDepth;

	void init();

public:
	WaterBuffers();
	virtual ~WaterBuffers() { ; }

	void bindReflectBuffer() const;
	void bindRefractBuffer() const;
	void updateBuffers();
	void removeBuffers();

	unsigned getReflectTextureId() { return reflectionFrame.getTextures()[0].getId(); }
	unsigned getRefractTextureId() { return refractionFrame.getTextures()[0].getId(); }
	unsigned getRefractDepthTextureId() { return refractDepth.getId(); }
};