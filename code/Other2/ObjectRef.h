#pragma once

// Reference to an object with id for serialization.
struct ObjectRef
{
<<<<<<< HEAD
	// Object id.
	unsigned id;

	// Construct with no reference.
	ObjectRef() :
		id(0)
	{
	}

	// Copy-construct.
	ObjectRef(const ObjectRef& ref) :
		id(ref.id)
	{
	}

	// Construct with object id.
	ObjectRef(unsigned id_) :
		id(id_)
	{
	}

	// Test for equality with another reference.
	bool operator==(const ObjectRef& rhs) const { return id == rhs.id; }
	// Test for inequality with another reference.
	bool operator!=(const ObjectRef& rhs) const { return !(*this == rhs); }
=======
	ObjectRef() = default;
	ObjectRef(const ObjectRef& ref) : id(ref.id) {}
	ObjectRef(unsigned id_) : id(id_) {}

	// Test for equality with another reference.
	bool operator==(const ObjectRef& rhs) const { return id == rhs.id; }
	// Test for inequality with another reference.
	bool operator!=(const ObjectRef& rhs) const { return !(*this == rhs); }

	// Object id.
	unsigned id = 0;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};