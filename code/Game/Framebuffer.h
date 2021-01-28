#pragma once

#include "OGLFunc.h"

class Framebuffer
{
public:
	~Framebuffer();

	void CreateFramebuffer();
	void CreateShadowFramebuffer();
	void ActivateShadowFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo); }
	void ActivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
	void DeactivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	GLuint GetColorBufferTexture() { return m_texture; }
	GLuint GetDepthBufferTexture() { return m_depthTexture; }

private:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;
	GLuint m_shadowFbo = 0;
	GLuint m_texture = 0;
	GLuint m_depthTexture = 0;
};