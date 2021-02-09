#pragma once

#include "OGLFunc.h"

class Framebuffer
{
public:
	void CreateFramebuffer(int width, int height);
	void DestroyFramebuffer();
	void ActivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
	void DeactivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	GLuint GetColorBufferTexture() const { return m_texture; }

private:
	GLuint m_fbo = 0u;
	GLuint m_rbo = 0u;
	GLuint m_texture = 0u;
};