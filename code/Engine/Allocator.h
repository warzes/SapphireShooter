#pragma once

struct AllocatorNode;

// Allocator memory block.
struct AllocatorBlock
{
	// Size of a node.
	size_t nodeSize;
	// Number of nodes in this block.
	size_t capacity;
	// First free node.
	AllocatorNode* free;
	// Next allocator block.
	AllocatorBlock* next;
	// Nodes follow.
};

<<<<<<< HEAD:code/Engine/Allocator.h
// Allocator node.
=======
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5:code/Other2/Allocator.h
struct AllocatorNode
{
	// Next free node.
	AllocatorNode* next;
	// Data follows.
};

// Initialize a fixed-size allocator with the node size and initial capacity.
AllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity = 1);
// Uninitialize a fixed-size allocator. Frees all blocks in the chain.
void AllocatorUninitialize(AllocatorBlock* allocator);
// Allocate a node. Creates a new block if necessary.
void* AllocatorGet(AllocatorBlock* allocator);
// Free a node. Does not free any blocks.
void AllocatorFree(AllocatorBlock* allocator, void* node);

// Allocator template class. Allocates objects of a specific class.
<<<<<<< HEAD:code/Engine/Allocator.h
template <class T> class Allocator
{
public:
	// Construct with optional initial capacity.
	Allocator(size_t capacity = 0) :
		allocator(nullptr)
=======
template <class T> 
class Allocator
{
public:
	Allocator(size_t capacity = 0) 
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5:code/Other2/Allocator.h
	{
		if (capacity)
			Reserve(capacity);
	}

<<<<<<< HEAD:code/Engine/Allocator.h
	// Destruct. All objects reserved from this allocator should be freed before this is called.
=======
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5:code/Other2/Allocator.h
	~Allocator()
	{
		Reset();
	}

	// Reserve initial capacity. Only possible before allocating the first object.
	void Reserve(size_t capacity)
	{
		if (!m_allocator)
			m_allocator = AllocatorInitialize(sizeof(T), capacity);
	}

	// Allocate and default-construct an object.
	T* Allocate()
	{
		if (!m_allocator)
			m_allocator = AllocatorInitialize(sizeof(T));
		T* newObject = static_cast<T*>(AllocatorGet(m_allocator));
		new(newObject) T();

		return newObject;
	}

	// Allocate and copy-construct an object.
	T* Allocate(const T& object)
	{
		if (!m_allocator)
			m_allocator = AllocatorInitialize(sizeof(T));
		T* newObject = static_cast<T*>(AllocatorGet(m_allocator));
		new(newObject) T(object);

		return newObject;
	}

	// Destruct and free an object.
	void Free(T* object)
	{
		(object)->~T();
		AllocatorFree(m_allocator, object);
	}

	// Free the allocator. All objects reserved from this allocator should be freed before this is called.
	void Reset()
	{
		AllocatorUninitialize(m_allocator);
		m_allocator = nullptr;
	}

private:
<<<<<<< HEAD:code/Engine/Allocator.h
	// Prevent copy construction.
	Allocator(const Allocator<T>& rhs);
	// Prevent assignment.
	Allocator<T>& operator=(const Allocator<T>& rhs);

	// Allocator block.
	AllocatorBlock* allocator;
=======
	Allocator(const Allocator<T>&) = delete;
	Allocator<T>& operator=(const Allocator<T>&) = delete;

	// Allocator block.
	AllocatorBlock* m_allocator = nullptr;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5:code/Other2/Allocator.h
};