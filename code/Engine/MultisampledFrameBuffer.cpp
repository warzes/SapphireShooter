#include "stdafx.h"
#if SE_OPENGL
#include "MultisampledFrameBuffer.h"

void MultisampledFrameBuffer::makeColorTextureBuffer(const unsigned& width, const unsigned& height)
{
	const unsigned ARRSIZE = colorTextures.size();
	//GLenum attachments[ARRSIZE]; // TODO
	std::vector<GLenum> attachments;
	attachments.resize(ARRSIZE);

	for (unsigned i = 0; i < ARRSIZE; ++i)
	{
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
		colorTextures[i].create();
		colorTextures[i].bind(GL_TEXTURE_2D_MULTISAMPLE);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D_MULTISAMPLE, colorTextures[i].getId(), 0);
	}
	glDrawBuffers(ARRSIZE, &attachments[0]);
	Texture::unbind(GL_TEXTURE_2D_MULTISAMPLE);
}
#endif