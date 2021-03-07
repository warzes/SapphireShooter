#include "stdafx.h"
#include "VectorBuffer.h"
#include "Engine/DebugNew.h"

VectorBuffer::VectorBuffer()
{
	SetName("Vector");
}

VectorBuffer::VectorBuffer(const Vector<unsigned char>& data)
{
	SetData(data);
	SetName("Vector");
}

VectorBuffer::VectorBuffer(const void* data, size_t numBytes)
{
	SetData(data, numBytes);
	SetName("Vector");
}

VectorBuffer::VectorBuffer(Stream& source, size_t numBytes)
{
	SetData(source, numBytes);
	SetName("Vector");
}

size_t VectorBuffer::Read(void* dest, size_t numBytes)
{
	if (numBytes + m_position > m_size)
		numBytes = m_size - m_position;
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

	return m_size;
}

size_t VectorBuffer::Seek(size_t newPosition)
{
	if (newPosition > m_size)
		newPosition = m_size;

	m_position = newPosition;
	return m_position;
}

size_t VectorBuffer::Write(const void* data, size_t numBytes)
{
	if (!numBytes)
		return 0;

	// Expand the buffer if necessary
	if (numBytes + m_position > m_size)
	{
		m_size = m_position + numBytes;
		buffer.Resize(m_size);
	}

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

bool VectorBuffer::IsReadable() const
{
	return true;
}

bool VectorBuffer::IsWritable() const
{
	return true;
}

void VectorBuffer::SetData(const Vector<unsigned char>& data)
{
	buffer = data;
	m_position = 0;
	m_size = data.Size();
}

void VectorBuffer::SetData(const void* data, size_t numBytes)
{
	if (!data)
		numBytes = 0;

	buffer.Resize(numBytes);
	if (numBytes)
		memcpy(&buffer[0], data, numBytes);

	m_position = 0;
	m_size = numBytes;
}

void VectorBuffer::SetData(Stream& source, size_t numBytes)
{
	buffer.Resize(numBytes);
	size_t actualSize = source.Read(&buffer[0], numBytes);
	if (actualSize != numBytes)
		buffer.Resize(actualSize);

	m_position = 0;
	m_size = actualSize;
}

void VectorBuffer::Clear()
{
	buffer.Clear();
	m_position = 0;
	m_size = 0;
}

void VectorBuffer::Resize(size_t newSize)
{
	buffer.Resize(newSize);
	m_size = newSize;
	if (m_position > m_size)
		m_position = m_size;
}