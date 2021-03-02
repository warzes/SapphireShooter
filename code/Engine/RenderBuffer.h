#pragma once

#include "OGLFunc.h"

class RenderBuffer
{
	void clear();
	void swap(const RenderBuffer& fb);

protected:
	unsigned RBO = 0;
	unsigned* amount;

public:
	RenderBuffer();
	RenderBuffer(const RenderBuffer& fb);
	RenderBuffer& operator=(const RenderBuffer& fb);
	virtual ~RenderBuffer();

	virtual void create(const unsigned& width, const unsigned& height);
	virtual void remove();
	virtual void attach() const;

	unsigned getId() const { return RBO; }
	bool isCreated() const { return RBO != 0; }

	void bind() const { glBindRenderbuffer(GL_RENDERBUFFER, RBO); }
	static void unbind() { glBindFramebuffer(GL_RENDERBUFFER, 0); }
};