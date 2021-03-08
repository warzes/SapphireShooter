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
template <class T> 
class Allocator
{
public:
	Allocator(size_t capacity = 0) 
	{
		if (capacity)
			Reserve(capacity);
	}

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
	Allocator(const Allocator<T>&) = delete;
	Allocator<T>& operator=(const Allocator<T>&) = delete;

	// Allocator block.
	AllocatorBlock* m_allocator = nullptr;
};