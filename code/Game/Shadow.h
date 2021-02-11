#pragma once

#include "FrameBuffer2.h"
#include "CubeDepthBuffer.h"
#include "Light.h"

class Shadow
{
	FrameBuffer depth;
	CubeDepthBuffer depthBuffer;

	void initCastShadow(const Light& light, const std::vector<Program*>& programs);

public:
	Shadow();
	virtual ~Shadow() { ; }

	void startCastShadow(const Light& light, const std::vector<Program*>& programs);
	void endCastShadow(const Light& light, const std::vector<Program*>& programs);
	void updateBuffer();
	ShadowTransforms generateShadowTransforms();
};