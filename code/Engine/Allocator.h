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
//  Reserve a node. Creates a new block if necessary.
void* AllocatorReserve(AllocatorBlock* allocator);
// Free a node. Does not free any blocks.
void AllocatorFree(AllocatorBlock* allocator, void* node);

// Allocator template class. Allocates objects of a specific class.
template <class T> 
class Allocator
{
public:
	Allocator(size_t initialCapacity = 0)
	{
		if (initialCapacity)
			m_allocator = AllocatorInitialize(sizeof(T), initialCapacity);
	}

	~Allocator()
	{
		AllocatorUninitialize(m_allocator);
	}

	/// Reserve and default-construct an object.
	T* Reserve()
	{
		if (!m_allocator)
			m_allocator = AllocatorInitialize(sizeof(T));
		auto* newObject = static_cast<T*>(AllocatorReserve(m_allocator));
		new(newObject) T();

		return newObject;
	}
	//// Allocate and default-construct an object.
	//T* Allocate()
	//{
	//	if (!m_allocator)
	//		m_allocator = AllocatorInitialize(sizeof(T));
	//	T* newObject = static_cast<T*>(AllocatorReserve(m_allocator));
	//	new(newObject) T();

	//	return newObject;
	//}

	/// Reserve and copy-construct an object.
	T* Reserve(const T& object)
	{
		if (!m_allocator)
			m_allocator = AllocatorInitialize(sizeof(T));
		auto* newObject = static_cast<T*>(AllocatorReserve(m_allocator));
		new(newObject) T(object);

		return newObject;
	}
	//// Allocate and copy-construct an object.
	//T* Allocate(const T& object)
	//{
	//	if (!m_allocator)
	//		m_allocator = AllocatorInitialize(sizeof(T));
	//	T* newObject = static_cast<T*>(AllocatorReserve(m_allocator));
	//	new(newObject) T(object);

	//	return newObject;
	//}

	//// Reserve initial capacity. Only possible before allocating the first object.
	//void Reserve(size_t capacity)
	//{
	//	if (!m_allocator)
	//		m_allocator = AllocatorInitialize(sizeof(T), capacity);
	//}
	
	// Destruct and free an object.
	void Free(T* object)
	{
		(object)->~T();
		AllocatorFree(m_allocator, object);
	}

	//// Free the allocator. All objects reserved from this allocator should be freed before this is called.
	//void Reset()
	//{
	//	AllocatorUninitialize(m_allocator);
	//	m_allocator = nullptr;
	//}

private:
	Allocator(const Allocator<T>&) = delete;
	Allocator<T>& operator=(const Allocator<T>&) = delete;

	// Allocator block.
	AllocatorBlock* m_allocator = nullptr;
};