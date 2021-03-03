#include "stdafx.h"
#if SE_OPENGL
#include "Profiler.h"
#include "GLGraphics.h"
#include "GLIndexBuffer.h"
#include "Engine/DebugNew.h"

IndexBuffer::IndexBuffer() :
	buffer(0),
	numIndices(0),
	indexSize(0),
	usage(USAGE_DEFAULT)
{
}

IndexBuffer::~IndexBuffer()
{
	Release();
}

void IndexBuffer::Release()
{
	if (graphics && graphics->GetIndexBuffer() == this)
		graphics->SetIndexBuffer(nullptr);

	if (buffer)
	{
		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}
}

void IndexBuffer::Recreate()
{
	if (numIndices)
	{
		Define(usage, numIndices, indexSize, !shadowData.IsNull(), shadowData.Get());
		SetDataLost(!shadowData.IsNull());
	}
}

bool IndexBuffer::SetData(size_t firstIndex, size_t numIndices_, const void* data)
{
	PROFILE(UpdateIndexBuffer);

	if (!data)
	{
		SE_LOG_ERROR("Null source data for updating index buffer");
		return false;
	}
	if (firstIndex + numIndices_ > numIndices)
	{
		SE_LOG_ERROR("Out of bounds range for updating index buffer");
		return false;
	}
	if (buffer && usage == USAGE_IMMUTABLE)
	{
		SE_LOG_ERROR("Can not update immutable index buffer");
		return false;
	}

	if (shadowData)
		memcpy(shadowData.Get() + firstIndex * indexSize, data, numIndices_ * indexSize);

	if (buffer)
	{
		graphics->SetIndexBuffer(this);
		if (numIndices_ == numIndices)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices_ * indexSize, data, usage == USAGE_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		else
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, firstIndex * indexSize, numIndices_ * indexSize, data);
	}

	return true;
}

bool IndexBuffer::Create(const void* data)
{
	glGenBuffers(1, &buffer);
	if (!buffer)
	{
		SE_LOG_ERROR("Failed to create index buffer");
		return false;
	}

	graphics->SetIndexBuffer(this);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * indexSize, data, usage == USAGE_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	SE_LOG("Created index buffer numIndices " + std::to_string(numIndices) + " indexSize " + std::to_string(indexSize));

	return true;
}

#endif // SE_OPENGL