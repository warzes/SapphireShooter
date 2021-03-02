#include "stdafx.h"
#include "Framebuffer.h"
#include "Log.h"
//-----------------------------------------------------------------------------
void Framebuffer::CreateFramebuffer(int width, int height)
{
	// Create and bind the new custom framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// Create new empty texture that will be rendered to then attach it to the framebuffer
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create render buffer to attach stencil and depth data to it 
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Ensure that the frame buffer is complete and ready to be used
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		SE_LOG("ERROR: Unable to create custom framebuffer.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-----------------------------------------------------------------------------
void Framebuffer::DestroyFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteTextures(1, &m_texture);
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_fbo);

	m_texture = 0;
	m_rbo = 0;
	m_fbo = 0;
}
//-----------------------------------------------------------------------------