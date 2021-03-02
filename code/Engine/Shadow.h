#pragma once

#include "FrameBuffer2.h"
#include "CubeDepthBuffer.h"
#include "Light.h"

class Shadow
{
	FrameBuffer depth;
	CubeDepthBuffer depthBuffer;

	void initCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs);

public:
	Shadow();
	virtual ~Shadow() { ; }

	void startCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs);
	void endCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs);
	void updateBuffer();
	ShadowTransforms generateShadowTransforms();
};