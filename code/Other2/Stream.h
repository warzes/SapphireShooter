#pragma once

#include "String.h"

class JSONValue;
class StringHash;
template <class T> class Vector;
struct ObjectRef;
struct ResourceRef;
struct ResourceRefList;

// Abstract stream for reading and writing.
class Stream
{
public:
	Stream() = default;
	Stream(size_t numBytes);
	virtual ~Stream() = default;

	// Read bytes from the stream. Return number of bytes actually read.
	virtual size_t Read(void* dest, size_t numBytes) = 0;
	// Set position in bytes from the beginning of the stream. Return the position after the seek.
	virtual size_t Seek(size_t position) = 0;
	// Write bytes to the stream. Return number of bytes actually written.
	virtual size_t Write(const void* data, size_t size) = 0;
	// Return whether read operations are allowed.
	virtual bool IsReadable() const = 0;
	// Return whether write operations are allowed.
	virtual bool IsWritable() const = 0;

	// Change the stream name.
	void SetName(const String& newName);
	// Change the stream name.
	void SetName(const char* newName);
	// Read a variable-length encoded unsigned integer, which can use 29 bits maximum.
	unsigned ReadVLE();
	// Read a text line.
	String ReadLine();
	// Read a 4-character file ID.
	String ReadFileID();
	// Read a byte buffer, with size prepended as a VLE value.
	Vector<unsigned char> ReadBuffer();
	// Write a four-letter file ID. If the string is not long enough, spaces will be appended.
	void WriteFileID(const String& value);
	// Write a byte buffer, with size encoded as VLE.
	void WriteBuffer(const Vector<unsigned char>& buffer);
	// Write a variable-length encoded unsigned integer, which can use 29 bits maximum.
	void WriteVLE(size_t value);
	// Write a text line. Char codes 13 & 10 will be automatically appended.
	void WriteLine(const String& value);

	// Write a value, template version.
	template <class T> void Write(const T& value) { Write(&value, sizeof value); }

	// Read a value, template version.
	template <class T> T Read()
	{
		T ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	// Return the stream name.
	const String& Name() const { return m_name; }
	// Return current position in bytes.
	size_t Position() const { return m_position; }
	// Return size in bytes.
	size_t Size() const { return m_size; }
	// Return whether the end of stream has been reached.
	bool IsEof() const { return m_position >= m_size; }

protected:
	size_t m_position = 0;
	size_t m_size = 0;
	String m_name;
};

template<> bool Stream::Read();
template<> String Stream::Read();
template<> StringHash Stream::Read();
template<> ResourceRef Stream::Read();
template<> ResourceRefList Stream::Read();
template<> ObjectRef Stream::Read();
template<> JSONValue Stream::Read();
template<> void Stream::Write(const bool& value);
template<> void Stream::Write(const String& value);
template<> void Stream::Write(const StringHash& value);
template<> void Stream::Write(const ResourceRef& value);
template<> void Stream::Write(const ResourceRefList& value);
template<> void Stream::Write(const ObjectRef& value);
template<> void Stream::Write(const JSONValue& value);