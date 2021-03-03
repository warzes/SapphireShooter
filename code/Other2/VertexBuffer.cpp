#include "stdafx.h"
#include "Profiler.h"
#include "VertexBuffer.h"
#include "Engine/DebugNew.h"

bool VertexBuffer::Define(ResourceUsage usage_, size_t numVertices_, const Vector<VertexElement>& elements_, bool useShadowData, const void* data)
{
	if (!numVertices_ || !elements_.Size())
	{
		SE_LOG_ERROR("Can not define vertex buffer with no vertices or no elements");
		return false;
	}

	return Define(usage_, numVertices_, elements_.Size(), &elements_[0], useShadowData, data);
}

bool VertexBuffer::Define(ResourceUsage usage_, size_t numVertices_, size_t numElements_, const VertexElement* elements_, bool useShadowData, const void* data)
{
	PROFILE(DefineVertexBuffer);

	if (!numVertices_ || !numElements_ || !elements_)
	{
		SE_LOG_ERROR("Can not define vertex buffer with no vertices or no elements");
		return false;
	}
	if (usage_ == USAGE_RENDERTARGET)
	{
		SE_LOG_ERROR("Rendertarget usage is illegal for vertex buffers");
		return false;
	}
	if (usage_ == USAGE_IMMUTABLE && !data)
	{
		SE_LOG_ERROR("Immutable vertex buffer must define initial data");
		return false;
	}

	for (size_t i = 0; i < numElements_; ++i)
	{
		if (elements_[i].type >= ELEM_MATRIX3X4)
		{
			SE_LOG_ERROR("Matrix elements are not supported in vertex buffers");
			return false;
		}
	}

	Release();

	numVertices = numVertices_;
	usage = usage_;

	// Determine offset of elements and the vertex size & element hash
	vertexSize = 0;
	elementHash = 0;
	elements.Resize(numElements_);
	for (size_t i = 0; i < numElements_; ++i)
	{
		elements[i] = elements_[i];
		elements[i].offset = vertexSize;
		vertexSize += elementSizes[elements[i].type];
		elementHash |= ElementHash(i, elements[i].semantic);
	}

	// If buffer is reinitialized with the same shadow data, no need to reallocate
	if (useShadowData && (!data || data != shadowData.Get()))
	{
		shadowData = new unsigned char[numVertices * vertexSize];
		if (data)
			memcpy(shadowData.Get(), data, numVertices * vertexSize);
	}

	return Create(data);
}