#include "stdafx.h"
#if SE_OPENGL
#include "Profiler.h"
#include "GLGraphics.h"
#include "GLVertexBuffer.h"
#include "Engine/DebugNew.h"

VertexBuffer::VertexBuffer() :
	buffer(0),
	numVertices(0),
	vertexSize(0),
	elementHash(0),
	usage(USAGE_DEFAULT)
{
}

VertexBuffer::~VertexBuffer()
{
	Release();
}

void VertexBuffer::Release()
{
	if (graphics)
	{
		for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
		{
			if (graphics->GetVertexBuffer(i) == this)
				graphics->SetVertexBuffer(i, 0);
		}
	}

	if (buffer)
	{
		if (graphics && graphics->BoundVBO() == buffer)
			graphics->BindVBO(0);

		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}
}

void VertexBuffer::Recreate()
{
	if (numVertices)
	{
		// Also make a copy of the current vertex elements, as they are passed by reference and manipulated by Define()
		Vector<VertexElement> srcElements = elements;
		Define(usage, numVertices, srcElements, !shadowData.IsNull(), shadowData.Get());
		SetDataLost(!shadowData.IsNull());
	}
}

bool VertexBuffer::SetData(size_t firstVertex, size_t numVertices_, const void* data)
{
	PROFILE(UpdateVertexBuffer);

	if (!data)
	{
		SE_LOG_ERROR("Null source data for updating vertex buffer");
		return false;
	}
	if (firstVertex + numVertices_ > numVertices)
	{
		SE_LOG_ERROR("Out of bounds range for updating vertex buffer");
		return false;
	}
	if (buffer && usage == USAGE_IMMUTABLE)
	{
		SE_LOG_ERROR("Can not update immutable vertex buffer");
		return false;
	}

	if (shadowData)
		memcpy(shadowData.Get() + firstVertex * vertexSize, data, numVertices_ * vertexSize);

	if (buffer)
	{
		graphics->BindVBO(buffer);
		if (numVertices_ == numVertices)
			glBufferData(GL_ARRAY_BUFFER, numVertices_ * vertexSize, data, usage == USAGE_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		else
			glBufferSubData(GL_ARRAY_BUFFER, firstVertex * vertexSize, numVertices_ * vertexSize, data);
	}

	return true;
}

bool VertexBuffer::Create(const void* data)
{
	glGenBuffers(1, &buffer);
	if (!buffer)
	{
		SE_LOG_ERROR("Failed to create vertex buffer");
		return false;
	}

	graphics->BindVBO(buffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, data, usage == USAGE_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	SE_LOG("Created vertex buffer numVertices " + std::to_string((unsigned)numVertices) + " vertexSize " + std::to_string((unsigned)vertexSize));

	return true;
}

#endif // SE_OPENGL