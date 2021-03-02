#include "stdafx.h"
#include "Allocator.h"
#include "Vector.h"
#include "Engine/DebugNew.h"

VectorBase::VectorBase() :
	buffer(nullptr)
{
}

void VectorBase::Swap(VectorBase& vector)
{
	::Swap(buffer, vector.buffer);
}

unsigned char* VectorBase::AllocateBuffer(size_t size)
{
	// Include space for size and capacity
	return new unsigned char[size + 2 * sizeof(size_t)];
}

template<> void Swap<VectorBase>(VectorBase& first, VectorBase& second)
{
	first.Swap(second);
}