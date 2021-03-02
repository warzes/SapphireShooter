#pragma once

#include "FrameBuffer2.h"

class MultisampledFrameBuffer : public FrameBuffer
{
protected:
    virtual void makeColorTextureBuffer(const unsigned& width, const unsigned& height);
};