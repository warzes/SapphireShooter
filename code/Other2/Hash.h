#pragma once

struct AllocatorBlock;

// Pointer hash function.
template <class T> unsigned MakeHash(T* value)
{
	return ((unsigned)(size_t)value) / sizeof(T);
}

// Const pointer hash function.
template <class T> unsigned MakeHash(const T* value)
{
	return ((unsigned)(size_t)value) / sizeof(T);
}

// Generic hash function.
template <class T> unsigned MakeHash(const T& value)
{
	return value.ToHash();
}

// Void pointer hash function.
template<> inline unsigned MakeHash(void* value)
{
	return (unsigned)(size_t)value;
}

// Const void pointer hash function.
template<> inline unsigned MakeHash(const void* value)
{
	return (unsigned)(size_t)value;
}

// Long long hash function.
template<> inline unsigned MakeHash(const long long& value)
{
	return (unsigned)((value >> 32) | (value & 0xffffffff));
}

// Unsigned long long hash function.
template<> inline unsigned MakeHash(const unsigned long long& value)
{
	return (value >> 32) | (value & 0xffffffff);
}

// Int hash function.
template<> inline unsigned MakeHash(const int& value)
{
	return (unsigned)value;
}

// Unsigned hash function.
template<> inline unsigned MakeHash(const unsigned& value)
{
	return value;
}

// Short hash function.
template<> inline unsigned MakeHash(const short& value)
{
	return (unsigned)value;
}

// Unsigned short hash function.
template<> inline unsigned MakeHash(const unsigned short& value)
{
	return value;
}

// Char hash function.
template<> inline unsigned MakeHash(const char& value)
{
	return (unsigned)value;
}

// Unsigned char hash function.
template<> inline unsigned MakeHash(const unsigned char& value)
{
	return value;
}

// Hash set/map node base class.
struct HashNodeBase
{
<<<<<<< HEAD
	// Construct.
	HashNodeBase() :
		down(nullptr),
		prev(nullptr),
		next(nullptr)
	{
	}

	// Next node in the bucket.
	HashNodeBase* down;
	// Previous node.
	HashNodeBase* prev;
	// Next node.
	HashNodeBase* next;
=======
	// Next node in the bucket.
	HashNodeBase* down = nullptr;
	// Previous node.
	HashNodeBase* prev = nullptr;
	// Next node.
	HashNodeBase* next = nullptr;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};

// Hash set/map iterator base class.
struct HashIteratorBase
{
<<<<<<< HEAD
	// Construct.
	HashIteratorBase() :
		ptr(nullptr)
	{
	}

	// Construct with a node pointer.
	explicit HashIteratorBase(HashNodeBase* ptr_) :
		ptr(ptr_)
	{
	}
=======
	HashIteratorBase() = default;
	explicit HashIteratorBase(HashNodeBase* ptr_) : ptr(ptr_) {}
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5

	// Test for equality with another iterator.
	bool operator==(const HashIteratorBase& rhs) const { return ptr == rhs.ptr; }
	// Test for inequality with another iterator.
	bool operator!=(const HashIteratorBase& rhs) const { return ptr != rhs.ptr; }

	// Go to the next node.
	void GotoNext()
	{
		if (ptr)
			ptr = ptr->next;
	}

	// Go to the previous node.
	void GotoPrev()
	{
		if (ptr)
			ptr = ptr->prev;
	}

	// Node pointer.
<<<<<<< HEAD
	HashNodeBase* ptr;
=======
	HashNodeBase* ptr = nullptr;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};

// Hash set/map base class.
class HashBase
{
public:
	// Initial amount of buckets.
	static const size_t MIN_BUCKETS = 8;
	// Maximum load factor.
	static const size_t MAX_LOAD_FACTOR = 4;

<<<<<<< HEAD
	// Construct.
	HashBase() :
		ptrs(nullptr),
		allocator(nullptr)
	{
	}

	// Destruct.
=======
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	~HashBase()
	{
		delete[] m_ptrs;
	}

	// Swap with another hash set or map.
	void Swap(HashBase& hash);

	// Return number of elements.
<<<<<<< HEAD
	size_t Size() const { return ptrs ? (reinterpret_cast<size_t*>(ptrs))[0] : 0; }
=======
	size_t Size() const { return m_ptrs ? (reinterpret_cast<size_t*>(m_ptrs))[0] : 0; }
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	// Return whether has no elements.
	bool IsEmpty() const { return Size() == 0; }

protected:
	// Allocate bucket head pointers + room for size and bucket count variables.
<<<<<<< HEAD
	void AllocateBuckets(size_t size, size_t numBuckets);
	// Reset bucket head pointers.
	void ResetPtrs();
	// Set new size.
	void SetSize(size_t size) { reinterpret_cast<size_t*>(ptrs)[0] = size; }
	// Set new head node.
	void SetHead(HashNodeBase* head) { ptrs[2] = head; }
	// Set new tail node.
	void SetTail(HashNodeBase* tail) { ptrs[3] = tail; }

	// Return number of buckets.
	size_t NumBuckets() const { return ptrs ? (reinterpret_cast<size_t*>(ptrs))[1] : MIN_BUCKETS; }
	// Return list head node.
	HashNodeBase* Head() const { return ptrs ? ptrs[2] : nullptr; }
	// Return list tail node.
	HashNodeBase* Tail() const { return ptrs ? ptrs[3] : nullptr; }
	// Return bucket head pointers.
	HashNodeBase** Ptrs() const { return ptrs ? ptrs + 4 : nullptr; }

	// Bucket head pointers.
	HashNodeBase** ptrs;
	// Node allocator.
	AllocatorBlock* allocator;
=======
	void allocateBuckets(size_t size, size_t numBuckets);
	// Reset bucket head pointers.
	void resetPtrs();
	// Set new size.
	void setSize(size_t size) { reinterpret_cast<size_t*>(m_ptrs)[0] = size; }
	// Set new head node.
	void setHead(HashNodeBase* head) { m_ptrs[2] = head; }
	// Set new tail node.
	void setTail(HashNodeBase* tail) { m_ptrs[3] = tail; }

	// Return number of buckets.
	size_t numBuckets() const { return m_ptrs ? (reinterpret_cast<size_t*>(m_ptrs))[1] : MIN_BUCKETS; }
	// Return list head node.
	HashNodeBase* head() const { return m_ptrs ? m_ptrs[2] : nullptr; }
	// Return list tail node.
	HashNodeBase* tail() const { return m_ptrs ? m_ptrs[3] : nullptr; }
	// Return bucket head pointers.
	HashNodeBase** ptrs() const { return m_ptrs ? m_ptrs + 4 : nullptr; }

	// Bucket head pointers.
	HashNodeBase** m_ptrs = nullptr;
	// Node allocator.
	AllocatorBlock* m_allocator = nullptr;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};