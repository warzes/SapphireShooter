#pragma once

#include "StringHash.h"
#include "Vector.h"

class Stream;

// Typed resource reference for serialization.
struct ResourceRef
{
	ResourceRef() = default;
	ResourceRef(const ResourceRef& ref) : type(ref.type), name(ref.name) {}
	
	ResourceRef(const String& str)
	{
		FromString(str);
	}

	ResourceRef(const char* str)
	{
		FromString(str);
	}

	ResourceRef(StringHash type, const String& name_ = String::EMPTY) : type(type), name(name_) {}

	// Set from a string that contains the type and name separated by a semicolon. Return true on success.
	bool FromString(const String& str);
	// Set from a C string that contains the type and name separated by a semicolon. Return true on success.
	bool FromString(const char* str);
	// Deserialize from a binary stream.
	void FromBinary(Stream& source);

	// Return as a string.
	String ToString() const;
	// Serialize to a binary stream.
	void ToBinary(Stream& dest) const;

	// Test for equality with another reference.
	bool operator==(const ResourceRef& rhs) const { return type == rhs.type && name == rhs.name; }
	// Test for inequality with another reference.
	bool operator!=(const ResourceRef& rhs) const { return !(*this == rhs); }

	// Resource type.
	StringHash type;
	// Resource name.
	String name;
};

// List of typed resource references for serialization.
struct ResourceRefList
{
	ResourceRefList() = default;
	ResourceRefList(const ResourceRefList& refList) : type(refList.type), names(refList.names) {}

	ResourceRefList(const String& str)
	{
		FromString(str);
	}

	ResourceRefList(const char* str)
	{
		FromString(str);
	}

	ResourceRefList(StringHash type, const Vector<String>& names_ = Vector<String>()) : type(type), names(names_) {}

	// Set from a string that contains the type and names separated by semicolons. Return true on success.
	bool FromString(const String& str);
	// Set from a C string that contains the type and names separated by semicolons. Return true on success.
	bool FromString(const char* str);
	// Deserialize from a binary stream.
	void FromBinary(Stream& source);

	// Return as a string.
	String ToString() const;
	// Deserialize from a binary stream.
	void ToBinary(Stream& dest) const;

	// Test for equality with another reference list.
	bool operator==(const ResourceRefList& rhs) const { return type == rhs.type && names == rhs.names; }
	// Test for inequality with another reference list.
	bool operator!=(const ResourceRefList& rhs) const { return !(*this == rhs); }

	// Resource type.
	StringHash type;
	// List of resource names.
	Vector<String> names;
};