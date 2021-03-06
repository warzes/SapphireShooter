#include "stdafx.h"
#include "Profiler.h"
#include "IndexBuffer.h"
#include "Engine/DebugNew.h"

bool IndexBuffer::Define(ResourceUsage usage_, size_t numIndices_, size_t indexSize_, bool useShadowData, const void* data)
{
	PROFILE(DefineIndexBuffer);

	Release();

	if (!numIndices_)
	{
		SE_LOG_ERROR("Can not define index buffer with no indices");
		return false;
	}
	if (usage_ == USAGE_RENDERTARGET)
	{
		SE_LOG_ERROR("Rendertarget usage is illegal for index buffers");
		return false;
	}
	if (usage_ == USAGE_IMMUTABLE && !data)
	{
		SE_LOG_ERROR("Immutable index buffer must define initial data");
		return false;
	}
	if (indexSize_ != sizeof(unsigned) && indexSize_ != sizeof(unsigned short))
	{
		SE_LOG_ERROR("Index buffer index size must be 2 or 4");
		return false;
	}

	numIndices = numIndices_;
	indexSize = indexSize_;
	usage = usage_;

	// If buffer is reinitialized with the same shadow data, no need to reallocate
	if (useShadowData && (!data || data != shadowData.Get()))
	{
		shadowData = new unsigned char[numIndices * indexSize];
		if (data)
			memcpy(shadowData.Get(), data, numIndices * indexSize);
	}

	return Create(data);
}