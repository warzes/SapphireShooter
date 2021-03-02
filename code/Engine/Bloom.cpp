#include "stdafx.h"
#include "Bloom.h"
#include "Engine.h"

Bloom::Bloom()
{
	unsigned width = GetEngineDescription().window.width;
	unsigned height = GetEngineDescription().window.height;
	pingPongBuffer[0].generate(width, height);
	pingPongBuffer[1].generate(width, height);
}

void Bloom::updateBuffers()
{
	unsigned width = GetEngineDescription().window.width;
	unsigned height = GetEngineDescription().window.height;

	pingPongBuffer[0].removeBuffers();
	pingPongBuffer[1].removeBuffers();
	pingPongBuffer[0].generate(width, height);
	pingPongBuffer[1].generate(width, height);
}

void Bloom::blurTexture(std::shared_ptr<ShaderProgram> blurProgram, const Texture& texture)
{
	horizontal = true;
	bool first_iteration = true;
	blurProgram->Bind();
	blurProgram->SetInteger("image", 0);
	for (unsigned i = 0; i < BLUR_AMOUNT; ++i)
	{
		pingPongBuffer[horizontal].bind();
		blurProgram->SetInteger("isHorizontal", horizontal);
		Texture::active(0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? texture.getId() : pingPongBuffer[!horizontal].getTextures()[0].getId());
		quad.Render(blurProgram);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}