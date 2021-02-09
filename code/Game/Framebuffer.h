#pragma once

#include "OGLFunc.h"

// TODO: удаление шадовбуфера
// TODO: возможно вообще удалить шадовбуфер

class Framebuffer
{
public:
	void CreateFramebuffer(int width, int height);
	//void CreateShadowFramebuffer();
	void DestroyFramebuffer();
	//void ActivateShadowFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo); }
	void ActivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
	void DeactivateFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	GLuint GetColorBufferTexture() const { return m_texture; }
	//GLuint GetDepthBufferTexture() const { return m_depthTexture; }

private:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;
	//GLuint m_shadowFbo = 0;
	GLuint m_texture = 0;
	//GLuint m_depthTexture = 0;
};