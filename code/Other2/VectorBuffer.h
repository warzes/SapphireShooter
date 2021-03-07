#pragma once

#if SE_UNUSED_ENABLE

#include "Vector.h"
#include "Stream.h"

// Dynamically sized buffer that can be read and written to as a stream.
class VectorBuffer : public Stream
{
public:
	VectorBuffer();
	VectorBuffer(const Vector<unsigned char>& data);
	VectorBuffer(const void* data, size_t numBytes);
	VectorBuffer(Stream& source, size_t numBytes);

	// Read bytes from the buffer. Return number of bytes actually read.
	size_t Read(void* dest, size_t size) override;
	// Set position in bytes from the beginning of the buffer.
	size_t Seek(size_t newPosition) override;
	// Write bytes to the buffer. Return number of bytes actually written.
	size_t Write(const void* data, size_t size) override;
	// Return whether read operations are allowed.
	bool IsReadable() const override;
	// Return whether write operations are allowed.
	bool IsWritable() const override;

	// Set data from another buffer.
	void SetData(const Vector<unsigned char>& data);
	// Set data from a memory area.
	void SetData(const void* data, size_t numBytes);
	// Set data from a stream.
	void SetData(Stream& source, size_t numBytes);
	// Reset to zero size.
	void Clear();
	// Set size.
	void Resize(size_t newSize);

	// Return data.
	const unsigned char* Data() const { return m_buffer.Begin().ptr; }
	// Return non-const data.
	unsigned char* ModifiableData() { return m_buffer.Begin().ptr; }
	// Return the buffer.
	const Vector<unsigned char>& Buffer() const { return m_buffer; }

	using Stream::Read;
	using Stream::Write;

private:
	// Dynamic data buffer.
	Vector<unsigned char> m_buffer;
};

#endif