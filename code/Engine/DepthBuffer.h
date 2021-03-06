#pragma once

#if SE_OPENGL

#include "Texture.h"

class DepthBuffer : public Texture
{
public:
	void create() { ; }
	virtual void create(const unsigned& width, const unsigned& height);
	virtual void attach() const;
};

#endif