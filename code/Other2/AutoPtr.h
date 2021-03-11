#pragma once

// Pointer which takes ownership of an object and deletes it when the pointer goes out of scope. Ownership can be transferred to another pointer, in which case the source pointer becomes null.
template <class T> 
class AutoPtr
{
public:
<<<<<<< HEAD
	// Construct a null pointer.
	AutoPtr() :
		ptr(nullptr)
	{
	}

	// Copy-construct. Ownership is transferred, making the source pointer null.
	AutoPtr(const AutoPtr<T>& ptr_) :
		ptr(ptr_.ptr)
=======
	AutoPtr() = default;

	// Copy-construct. Ownership is transferred, making the source pointer null.
	AutoPtr(const AutoPtr<T>& ptr_) : m_ptr(ptr_.m_ptr)
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	{
		// Trick the compiler so that the AutoPtr can be copied to containers; the latest copy stays non-null
		const_cast<AutoPtr<T>&>(ptr_).m_ptr = nullptr;
	}

	// Construct with a raw pointer; take ownership of the object.
<<<<<<< HEAD
	AutoPtr(T* ptr_) :
		ptr(ptr_)
	{
	}

	// Destruct. Delete the object pointed to.
=======
	AutoPtr(T* ptr_) : m_ptr(ptr_) {}

>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	~AutoPtr()
	{
		delete m_ptr;
	}

	// Assign from a pointer. Existing object is deleted and ownership is transferred from the source pointer, which becomes null.
	AutoPtr<T>& operator=(const AutoPtr<T>& rhs)
	{
		delete m_ptr;
		m_ptr = rhs.m_ptr;
		const_cast<AutoPtr<T>&>(rhs).m_ptr = nullptr;
		return *this;
	}

	// Assign a new object. Existing object is deleted.
	AutoPtr<T>& operator=(T* rhs)
	{
		delete m_ptr;
		m_ptr = rhs;
		return *this;
	}

	// Detach the object from the pointer without destroying it and return it. The pointer becomes null.
	T* Detach()
	{
		T* ret = m_ptr;
		m_ptr = nullptr;
		return ret;
	}

	// Reset to null. Destroys the object.
	void Reset()
	{
		*this = nullptr;
	}

	// Point to the object.
<<<<<<< HEAD
	T* operator->() const { assert(ptr); return ptr; }
	// Dereference the object.
	T& operator*() const { assert(ptr); return *ptr; }
	// Convert to the object.
	operator T* () const { return ptr; }

	// Return the object.
	T* Get() const { return ptr; }
	// Return whether is a null pointer.
	bool IsNull() const { return ptr == nullptr; }

private:
	// Object pointer.
	T* ptr;
};

// Pointer which takes ownership of an array allocated with new[] and deletes it when the pointer goes out of scope.
template <class T> class AutoArrayPtr
{
public:
	// Construct a null pointer.
	AutoArrayPtr() :
		array(nullptr)
	{
	}

	// Copy-construct. Ownership is transferred, making the source pointer null.
	AutoArrayPtr(AutoArrayPtr<T>& ptr) :
		array(ptr.array)
=======
	T* operator->() const { assert(m_ptr); return m_ptr; }
	// Dereference the object.
	T& operator*() const { assert(m_ptr); return *m_ptr; }
	// Convert to the object.
	operator T*() const { return m_ptr; }

	// Return the object.
	T* Get() const { return m_ptr; }
	// Return whether is a null pointer.
	bool IsNull() const { return m_ptr == nullptr; }

private:
	// Object pointer.
	T* m_ptr = nullptr;
};

// Pointer which takes ownership of an array allocated with new[] and deletes it when the pointer goes out of scope.
template <class T> 
class AutoArrayPtr
{
public:
	AutoArrayPtr() = default;

	// Copy-construct. Ownership is transferred, making the source pointer null.
	AutoArrayPtr(AutoArrayPtr<T>& ptr) : m_array(ptr.m_array)
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	{
		ptr.m_array = nullptr;
	}

	// Construct and take ownership of the array.
<<<<<<< HEAD
	AutoArrayPtr(T* array_) :
		array(array_)
=======
	AutoArrayPtr(T* array_) : m_array(array_)
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	{
	}

	// Destruct. Delete the array pointed to.
	~AutoArrayPtr()
	{
		delete[] m_array;
	}

	// Assign from a pointer. Existing array is deleted and ownership is transferred from the source pointer, which becomes null.
	AutoArrayPtr<T>& operator=(AutoArrayPtr<T>& rhs)
	{
		delete m_array;
		m_array = rhs.m_array;
		rhs.m_array = nullptr;
		return *this;
	}

	// Assign a new array. Existing array is deleted.
	AutoArrayPtr<T>& operator=(T* rhs)
	{
		delete m_array;
		m_array = rhs;
		return *this;
	}

	// Detach the array from the pointer without destroying it and return it. The pointer becomes null.
	T* Detach()
	{
		T* ret = m_array;
		m_array = nullptr;
		return ret;
	}

	// Reset to null. Destroys the array.
	void Reset()
	{
		*this = nullptr;
	}

	// Point to the array.
<<<<<<< HEAD
	T* operator->() const { assert(array); return array; }
	// Dereference the array.
	T& operator*() const { assert(array); return *array; }
	// Index the array.
	T& operator [] (size_t index) { assert(array); return array[index]; }
	// Const-index the array.
	const T& operator [] (size_t index) const { assert(array); return array[index]; }
	// Convert to bool.
	operator bool() const { return array != nullptr; }

	// Return the array.
	T* Get() const { return array; }
	// Return whether is a null pointer.
	bool IsNull() const { return array == nullptr; }

private:
	// Array pointer.
	T* array;
=======
	T* operator->() const { assert(m_array); return m_array; }
	// Dereference the array.
	T& operator*() const { assert(m_array); return *m_array; }
	// Index the array.
	T& operator[](size_t index) { assert(m_array); return m_array[index]; }
	// Const-index the array.
	const T& operator[](size_t index) const { assert(m_array); return m_array[index]; }
	// Convert to bool.
	operator bool() const { return m_array != nullptr; }

	// Return the array.
	T* Get() const { return m_array; }
	// Return whether is a null pointer.
	bool IsNull() const { return m_array == nullptr; }

private:
	// Array pointer.
	T* m_array = nullptr;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};