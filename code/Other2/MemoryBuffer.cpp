#include "stdafx.h"
#include "Vector.h"
#include "MemoryBuffer.h"
#include "Engine/DebugNew.h"

MemoryBuffer::MemoryBuffer(void* data, size_t numBytes) :
	Stream(data ? numBytes : 0),
	buffer((unsigned char*)data),
	readOnly(false)
{
	SetName("Memory");
}

MemoryBuffer::MemoryBuffer(const void* data, size_t numBytes) :
	Stream(data ? numBytes : 0),
	buffer((unsigned char*)data),
	readOnly(true)
{
	SetName("Memory");
}

MemoryBuffer::MemoryBuffer(Vector<unsigned char>& data) :
	Stream(data.Size()),
	buffer(data.Begin().ptr),
	readOnly(false)
{
}

MemoryBuffer::MemoryBuffer(const Vector<unsigned char>& data) :
	Stream(data.Size()),
	buffer(data.Begin().ptr),
	readOnly(true)
{
}

size_t MemoryBuffer::Read(void* dest, size_t numBytes)
{
	if (numBytes + m_position > size)
		numBytes = size - m_position;
	if (!numBytes)
		return 0;

	unsigned char* srcPtr = &buffer[m_position];
	unsigned char* destPtr = (unsigned char*)dest;
	m_position += numBytes;

	size_t copySize = numBytes;
	while (copySize >= sizeof(unsigned))
	{
		*((unsigned*)destPtr) = *((unsigned*)srcPtr);
		srcPtr += sizeof(unsigned);
		destPtr += sizeof(unsigned);
		copySize -= sizeof(unsigned);
	}
	if (copySize & sizeof(unsigned short))
	{
		*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
		srcPtr += sizeof(unsigned short);
		destPtr += sizeof(unsigned short);
	}
	if (copySize & 1)
		*destPtr = *srcPtr;

	return numBytes;
}

size_t MemoryBuffer::Seek(size_t newPosition)
{
	if (newPosition > size)
		newPosition = size;

	m_position = newPosition;
	return m_position;
}

size_t MemoryBuffer::Write(const void* data, size_t numBytes)
{
	if (numBytes + m_position > size)
		numBytes = size - m_position;
	if (!numBytes || readOnly)
		return 0;

	unsigned char* srcPtr = (unsigned char*)data;
	unsigned char* destPtr = &buffer[m_position];
	m_position += numBytes;

	size_t copySize = numBytes;
	while (copySize >= sizeof(unsigned))
	{
		*((unsigned*)destPtr) = *((unsigned*)srcPtr);
		srcPtr += sizeof(unsigned);
		destPtr += sizeof(unsigned);
		copySize -= sizeof(unsigned);
	}
	if (copySize & sizeof(unsigned short))
	{
		*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
		srcPtr += sizeof(unsigned short);
		destPtr += sizeof(unsigned short);
	}
	if (copySize & 1)
		*destPtr = *srcPtr;

	return numBytes;
}

bool MemoryBuffer::IsReadable() const
{
	return buffer != nullptr;
}

bool MemoryBuffer::IsWritable() const
{
	return buffer && !readOnly;
}