#include "stdafx.h"
#include "Stream.h"
#include "Math.h"
#include "JSONValue.h"
#include "ObjectRef.h"
#include "ResourceRef.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
Stream::Stream(size_t numBytes) 
	: m_size(numBytes)
{
}
//-----------------------------------------------------------------------------
void Stream::SetName(const String& newName)
{
	m_name = newName;
}
//-----------------------------------------------------------------------------
void Stream::SetName(const char* newName)
{
	m_name = newName;
}
//-----------------------------------------------------------------------------
unsigned Stream::ReadVLE()
{
	unsigned ret;
	unsigned char byte;

	byte = Read<unsigned char>();
	ret = byte & 0x7fU;
	if (byte < 0x80)
		return ret;

	byte = Read<unsigned char>();
	ret |= ((unsigned)(byte & 0x7fU)) << 7;
	if (byte < 0x80U)
		return ret;

	byte = Read<unsigned char>();
	ret |= ((unsigned)(byte & 0x7fU)) << 14;
	if (byte < 0x80U)
		return ret;

	byte = Read<unsigned char>();
	ret |= ((unsigned)byte) << 21U;
	return ret;
}
//-----------------------------------------------------------------------------
String Stream::ReadLine()
{
	String ret;

	while (!IsEof())
	{
		char c = Read<char>();
		if (c == 10)
			break;
		if (c == 13)
		{
			// Peek next char to see if it's 10, and skip it too
			if (!IsEof())
			{
				char next = Read<char>();
				if (next != 10)
					Seek(m_position - 1);
			}
			break;
		}

		ret += c;
	}

	return ret;
}
//-----------------------------------------------------------------------------
String Stream::ReadFileID()
{
	String ret;
	ret.Resize(4);
	Read(&ret[0], 4);
	return ret;
}
//-----------------------------------------------------------------------------
Vector<unsigned char> Stream::ReadBuffer()
{
	Vector<unsigned char> ret(ReadVLE());
	if (ret.Size())
		Read(&ret[0], ret.Size());
	return ret;
}
//-----------------------------------------------------------------------------
template<> bool Stream::Read<bool>()
{
	return Read<unsigned char>() != 0;
}
//-----------------------------------------------------------------------------
template<> String Stream::Read<String>()
{
	String ret;

	while (!IsEof())
	{
		char c = Read<char>();
		if (!c)
			break;
		else
			ret += c;
	}

	return ret;
}
//-----------------------------------------------------------------------------
template<> StringHash Stream::Read<StringHash>()
{
	return StringHash(Read<unsigned>());
}
//-----------------------------------------------------------------------------
template<> ResourceRef Stream::Read<ResourceRef>()
{
	ResourceRef ret;
	ret.FromBinary(*this);
	return ret;
}
//-----------------------------------------------------------------------------
template<> ResourceRefList Stream::Read<ResourceRefList>()
{
	ResourceRefList ret;
	ret.FromBinary(*this);
	return ret;
}
//-----------------------------------------------------------------------------
template<> ObjectRef Stream::Read<ObjectRef>()
{
	ObjectRef ret;
	ret.id = Read<unsigned>();
	return ret;
}
//-----------------------------------------------------------------------------
template<> JSONValue Stream::Read<JSONValue>()
{
	JSONValue ret;
	ret.FromBinary(*this);
	return ret;
}
//-----------------------------------------------------------------------------
void Stream::WriteFileID(const String& value)
{
	Write(value.CString(), Min(value.Length(), 4ULL));
	for (size_t i = value.Length(); i < 4; ++i)
		Write(' ');
}
//-----------------------------------------------------------------------------
void Stream::WriteBuffer(const Vector<unsigned char>& value)
{
	size_t numBytes = value.Size();

	WriteVLE(numBytes);
	if (numBytes)
		Write(&value[0], numBytes);
}
//-----------------------------------------------------------------------------
void Stream::WriteVLE(size_t value)
{
	unsigned char data[4];

	if (value < 0x80)
		Write((unsigned char)value);
	else if (value < 0x4000)
	{
		data[0] = (unsigned char)value | 0x80U;
		data[1] = (unsigned char)(value >> 7);
		Write(data, 2);
	}
	else if (value < 0x200000)
	{
		data[0] = (unsigned char)value | 0x80U;
		data[1] = (unsigned char)((value >> 7) | 0x80U);
		data[2] = (unsigned char)(value >> 14);
		Write(data, 3);
	}
	else
	{
		data[0] = (unsigned char)value | 0x80U;
		data[1] = (unsigned char)((value >> 7) | 0x80U);
		data[2] = (unsigned char)((value >> 14) | 0x80U);
		data[3] = (unsigned char)(value >> 21);
		Write(data, 4);
	}
}
//-----------------------------------------------------------------------------
void Stream::WriteLine(const String& value)
{
	Write(value.CString(), value.Length());
	Write('\r');
	Write('\n');
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<bool>(const bool& value)
{
	Write<unsigned char>(value ? 1u : 0u);
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<String>(const String& value)
{
	// Write content and null terminator
	Write(value.CString(), value.Length() + 1);
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<StringHash>(const StringHash& value)
{
	Write(value.Value());
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<ResourceRef>(const ResourceRef& value)
{
	value.ToBinary(*this);
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<ResourceRefList>(const ResourceRefList& value)
{
	value.ToBinary(*this);
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<ObjectRef>(const ObjectRef& value)
{
	Write(value.id);
}
//-----------------------------------------------------------------------------
template<> void Stream::Write<JSONValue>(const JSONValue& value)
{
	value.ToBinary(*this);
}
//-----------------------------------------------------------------------------