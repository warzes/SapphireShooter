#include "stdafx.h"
#if SE_OPENGL
#include "WaterBuffers.h"
#include "Engine.h"

void WaterBuffers::init()
{
	auto width = GetEngineDescription().window.width;
	auto height = GetEngineDescription().window.height;

	reflectionFrame.setRenderBuffer(reflectRenderBuffer);
	reflectionFrame.generate(width, height);

	refractionFrame.setDepthBuffer(refractDepth);
	refractionFrame.generate(width, height);
}

WaterBuffers::WaterBuffers()
{
	init();
}

void WaterBuffers::updateBuffers()
{
	removeBuffers();
	init();
}

void WaterBuffers::removeBuffers()
{
	reflectionFrame.removeBuffers();
	refractionFrame.removeBuffers();
}

void WaterBuffers::bindReflectBuffer() const
{
	reflectionFrame.bind();
}

void WaterBuffers::bindRefractBuffer() const
{
	refractionFrame.bind();
}
#endif