#include "stdafx.h"
#include "MultisampledRenderBuffer.h"

void MultisampledRenderBuffer::create(const unsigned& width, const unsigned& height)
{
	glGenRenderbuffers(1, &RBO);
	bind();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	unbind();
}