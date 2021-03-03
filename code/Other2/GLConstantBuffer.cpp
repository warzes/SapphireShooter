#include "stdafx.h"
#if SE_OPENGL
#include "Profiler.h"
#include "Color.h"
#include "Matrix3.h"
#include "Matrix3x4.h"
#include "GLConstantBuffer.h"
#include "GLGraphics.h"
#include "Engine/DebugNew.h"

ConstantBuffer::ConstantBuffer() :
	buffer(0),
	byteSize(0),
	usage(USAGE_DEFAULT),
	dirty(false)
{
}

ConstantBuffer::~ConstantBuffer()
{
	Release();
}

void ConstantBuffer::Release()
{
	if (graphics)
	{
		for (size_t i = 0; i < MAX_SHADER_STAGES; ++i)
		{
			for (size_t j = 0; j < MAX_CONSTANT_BUFFERS; ++j)
			{
				if (graphics->GetConstantBuffer((ShaderStage)i, j) == this)
					graphics->SetConstantBuffer((ShaderStage)i, j, 0);
			}
		}
	}

	if (buffer)
	{
		if (graphics && graphics->BoundUBO() == buffer)
			graphics->BindUBO(0);

		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}
}

void ConstantBuffer::Recreate()
{
	if (constants.Size())
	{
		// Make a copy of the current constants, as they are passed by reference and manipulated by Define()
		Vector<Constant> srcConstants = constants;
		Define(usage, srcConstants);
		Apply();
	}
}

bool ConstantBuffer::SetData(const void* data, bool copyToShadow)
{
	if (copyToShadow)
		memcpy(shadowData.Get(), data, byteSize);

	if (usage == USAGE_IMMUTABLE)
	{
		if (!buffer)
			return Create(data);
		else
		{
			SE_LOG_ERROR("Apply can only be called once on an immutable constant buffer");
			return false;
		}
	}

	if (buffer)
	{
		graphics->BindUBO(buffer);
		glBufferData(GL_UNIFORM_BUFFER, byteSize, data, usage != USAGE_IMMUTABLE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	dirty = false;
	return true;
}

bool ConstantBuffer::Create(const void* data)
{
	dirty = false;

	glGenBuffers(1, &buffer);
	if (!buffer)
	{
		SE_LOG_ERROR("Failed to create constant buffer");
		return false;
	}

	graphics->BindUBO(buffer);
	glBufferData(GL_UNIFORM_BUFFER, byteSize, data, usage != USAGE_IMMUTABLE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	return true;
}

#endif // SE_OPENGL