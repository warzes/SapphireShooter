#pragma once

// Reference to an object with id for serialization.
struct ObjectRef
{
	ObjectRef() = default;
	ObjectRef(const ObjectRef& ref) : id(ref.id) {}
	ObjectRef(unsigned id_) : id(id_) {}

	// Test for equality with another reference.
	bool operator==(const ObjectRef& rhs) const { return id == rhs.id; }
	// Test for inequality with another reference.
	bool operator!=(const ObjectRef& rhs) const { return !(*this == rhs); }

	// Object id.
	unsigned id = 0;
};