#pragma once

#include "Plane.h"
#include "FrameBuffer2.h"

class Bloom
{
	const unsigned BLUR_AMOUNT = 8;
	FrameBuffer pingPongBuffer[2];
	Plane quad;
	bool horizontal;

public:
	Bloom();
	virtual ~Bloom() { ; }

	void updateBuffers();
	void blurTexture(std::shared_ptr<ShaderProgram> blurProgram, const Texture& texture);
	const Texture& getResultTexture() const { return pingPongBuffer[!horizontal].getTextures()[0]; }
};