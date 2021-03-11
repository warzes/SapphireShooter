#pragma once

#include "StringHash.h"
#include "Vector.h"

class Stream;

// Typed resource reference for serialization.
struct ResourceRef
{
<<<<<<< HEAD
	// Resource type.
	StringHash type;
	// Resource name.
	String name;

	// Construct.
	ResourceRef()
	{
	}

	// Copy-construct.
	ResourceRef(const ResourceRef& ref) :
		type(ref.type),
		name(ref.name)
	{
	}

	// Construct from a string.
=======
	ResourceRef() = default;
	ResourceRef(const ResourceRef& ref) : type(ref.type), name(ref.name) {}
	
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	ResourceRef(const String& str)
	{
		FromString(str);
	}

<<<<<<< HEAD
	// Construct from a C string.
=======
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	ResourceRef(const char* str)
	{
		FromString(str);
	}

<<<<<<< HEAD
	// Construct with type and resource name.
	ResourceRef(StringHash type, const String& name_ = String::EMPTY) :
		type(type),
		name(name_)
	{
	}
=======
	ResourceRef(StringHash type, const String& name_ = String::EMPTY) : type(type), name(name_) {}
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5

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
<<<<<<< HEAD
=======

	// Resource type.
	StringHash type;
	// Resource name.
	String name;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};

// List of typed resource references for serialization.
struct ResourceRefList
{
<<<<<<< HEAD
	// Resource type.
	StringHash type;
	// List of resource names.
	Vector<String> names;

	// Construct.
	ResourceRefList()
	{
	}

	// Copy-construct.
	ResourceRefList(const ResourceRefList& refList) :
		type(refList.type),
		names(refList.names)
	{
	}

	// Construct from a string.
=======
	ResourceRefList() = default;
	ResourceRefList(const ResourceRefList& refList) : type(refList.type), names(refList.names) {}

>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	ResourceRefList(const String& str)
	{
		FromString(str);
	}

<<<<<<< HEAD
	// Construct from a C string.
=======
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	ResourceRefList(const char* str)
	{
		FromString(str);
	}

<<<<<<< HEAD
	// Construct with type and name list.
	ResourceRefList(StringHash type, const Vector<String>& names_ = Vector<String>()) :
		type(type),
		names(names_)
	{
	}
=======
	ResourceRefList(StringHash type, const Vector<String>& names_ = Vector<String>()) : type(type), names(names_) {}
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5

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
<<<<<<< HEAD
=======

	// Resource type.
	StringHash type;
	// List of resource names.
	Vector<String> names;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};