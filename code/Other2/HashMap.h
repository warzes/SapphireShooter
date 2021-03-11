#pragma once

#include "Engine/Allocator.h"
#include "Pair.h"
#include "Sort.h"
#include "Vector.h"

// hash map template class.
template <class T, class U> 
class HashMap : public HashBase
{
public:
	// hash map key-value pair with const key.
	class KeyValue
	{
	public:
		KeyValue() : first(T()) {}
		KeyValue(const T& key, const U& value) : first(key), second(value) {}

		// Test for equality with another pair.
		bool operator==(const KeyValue& rhs) const { return first == rhs.first && second == rhs.second; }
		// Test for inequality with another pair.
		bool operator!=(const KeyValue& rhs) const { return !(*this == rhs); }

		// Key.
		const T first;
		// Value.
		U second;

	private:
		KeyValue(const KeyValue&) = delete;
		KeyValue& operator=(const KeyValue&) = delete;
	};

	// hash map node.
	struct Node : public HashNodeBase
	{
		Node() = default;
		Node(const T& key, const U& value) : pair(key, value) {}

		// Return next node.
		Node* Next() const { return static_cast<Node*>(next); }
		// Return previous node.
		Node* Prev() const { return static_cast<Node*>(prev); }
		// Return next node in the bucket.
		Node* Down() const { return static_cast<Node*>(down); }

		// Key-value pair.
		KeyValue pair;
	};

	// hash map node iterator.
	struct Iterator : public HashIteratorBase
	{
		Iterator() = default;
		Iterator(Node* ptr) : HashIteratorBase(ptr) {}

		// Preincrement the pointer.
		Iterator& operator++() { GotoNext(); return *this; }
		// Postincrement the pointer.
		Iterator operator++(int) { Iterator it = *this; GotoNext(); return it; }
		// Predecrement the pointer.
		Iterator& operator--() { GotoPrev(); return *this; }
		// Postdecrement the pointer.
		Iterator operator--(int) { Iterator it = *this; GotoPrev(); return it; }

		// Point to the pair.
		KeyValue* operator->() const { return &(static_cast<Node*>(ptr))->pair; }
		// Dereference the pair.
		KeyValue& operator*() const { return (static_cast<Node*>(ptr))->pair; }
	};

	// hash map node const iterator.
	struct ConstIterator : public HashIteratorBase
	{
		ConstIterator() = default;
		ConstIterator(Node* ptr) : HashIteratorBase(ptr) {}
		ConstIterator(const Iterator& rhs) : HashIteratorBase(rhs.ptr) {}

		// Assign from a non-const iterator.
		ConstIterator& operator=(const Iterator& rhs) { ptr = rhs.ptr; return *this; }
		// Preincrement the pointer.
		ConstIterator& operator++() { GotoNext(); return *this; }
		// Postincrement the pointer.
		ConstIterator operator++(int) { ConstIterator it = *this; GotoNext(); return it; }
		// Predecrement the pointer.
		ConstIterator& operator--() { GotoPrev(); return *this; }
		// Postdecrement the pointer.
		ConstIterator operator--(int) { ConstIterator it = *this; GotoPrev(); return it; }

		// Point to the pair.
		const KeyValue* operator->() const { return &(static_cast<Node*>(ptr))->pair; }
		// Dereference the pair.
		const KeyValue& operator*() const { return (static_cast<Node*>(ptr))->pair; }
	};

	HashMap() = default;

	HashMap(const HashMap<T, U>& map)
	{
		Initialize(map.NumBuckets(), map.Size() + 1);
		*this = map;
	}

	~HashMap()
	{
		if (m_ptrs && m_allocator)
		{
			Clear();
			freeNode(tail());
			AllocatorUninitialize(m_allocator);
		}
	}

	HashMap& operator=(const HashMap<T, U>& rhs)
	{
		if (&rhs != this)
		{
			Clear();
			Insert(rhs);
		}
		return *this;
	}

	// Add-assign a pair.
	HashMap& operator+=(const Pair<T, U>& rhs)
	{
		Insert(rhs);
		return *this;
	}

	// Add-assign a hash map.
	HashMap& operator+=(const HashMap<T, U>& rhs)
	{
		Insert(rhs);
		return *this;
	}

	// Test for equality with another hash map.
	bool operator==(const HashMap<T, U>& rhs) const
	{
		if (rhs.Size() != Size())
			return false;

		for (ConstIterator it = Begin(); it != End(); ++it)
		{
			ConstIterator rhsIt = rhs.Find(it->first);
			if (rhsIt == rhs.End() || rhsIt->second != it->second)
				return false;
		}

		return true;
	}

	// Test for inequality with another hash map.
	bool operator!=(const HashMap<T, U>& rhs) const { return !(*this == rhs); }

	// Index the map. Create a new pair if key not found.
	U& operator[](const T& key)
	{
		return insertNode(key)->pair.second;
	}

	// Insert a pair. Return an iterator to it.
	Iterator Insert(const Pair<T, U>& pair)
	{
		return Iterator(insertNode(pair.first, pair.second));
	}

	// Insert a map.
	void Insert(const HashMap<T, U>& map)
	{
		for (ConstIterator it = map.Begin(); it != map.End(); ++it)
			insertNode(it->first, it->second);
	}

	// Insert a pair by iterator. Return iterator to the value.
	Iterator Insert(const ConstIterator& it) { return Iterator(insertNode(it->first, it->second)); }

	// Insert a range by iterators.
	void Insert(const ConstIterator& start, const ConstIterator& end)
	{
		ConstIterator it = start;
		while (it != end)
			Insert(*it++);
	}

	// Erase a pair by key. Return true if was found.
	bool Erase(const T& key)
	{
		if (!m_ptrs)
			return false;

		unsigned hashKey = hash(key);

		Node* previous;
		Node* node = findNode(key, hashKey, previous);
		if (!node)
			return false;

		if (previous)
			previous->down = node->down;
		else
			ptrs()[hashKey] = node->down;

		eraseNode(node);
		return true;
	}

	// Erase a pair by iterator. Return iterator to the next pair.
	Iterator Erase(const Iterator& it)
	{
		if (!m_ptrs || !it.ptr)
			return End();

		Node* node = static_cast<Node*>(it.ptr);
		Node* next = node->Next();

		unsigned hashKey = hash(node->pair.first);

		Node* previous = nullptr;
		Node* current = static_cast<Node*>(ptrs()[hashKey]);
		while (current && current != node)
		{
			previous = current;
			current = current->Down();
		}

		assert(current == node);

		if (previous)
			previous->down = node->down;
		else
			ptrs()[hashKey] = node->down;

		eraseNode(node);
		return Iterator(next);
	}

	// Clear the map.
	void Clear()
	{
		if (Size())
		{
			for (Iterator it = Begin(); it != End();)
				freeNode(static_cast<Node*>(it++.ptr));

			Node* t = tail();
			t->prev = nullptr;
			setHead(t);
			setSize(0);
			resetPtrs();
		}
	}

	// Sort pairs. After sorting the map can be iterated in order until new elements are inserted.
	void Sort()
	{
		size_t numKeys = Size();
		if (!numKeys)
			return;

		Node** ptrs = new Node * [numKeys];
		Node* ptr = head();

		for (size_t i = 0; i < numKeys; ++i)
		{
			ptrs[i] = ptr;
			ptr = ptr->Next();
		}

		::Sort(RandomAccessIterator<Node*>(ptrs), RandomAccessIterator<Node*>(ptrs + numKeys), compareNodes);

		setHead(ptrs[0]);
		ptrs[0]->prev = nullptr;
		for (size_t i = 1; i < numKeys; ++i)
		{
			ptrs[i - 1]->next = ptrs[i];
			ptrs[i]->prev = ptrs[i - 1];
		}
		ptrs[numKeys - 1]->next = tail();
		tail()->prev = ptrs[numKeys - 1];

		delete[] ptrs;
	}

	// Rehash to a specific bucket count, which must be a power of two. Return true on success.
	bool Rehash(size_t numBucket)
	{
		if (numBucket == numBuckets())
			return true;
		if (!numBucket || numBucket < Size() / MAX_LOAD_FACTOR)
			return false;

		// Check for being power of two
		size_t check = numBucket;
		while (!(check & 1))
			check >>= 1;
		if (check != 1)
			return false;

		allocateBuckets(Size(), numBucket);
		Rehash();
		return true;
	}

	// Return iterator to the pair with key, or end iterator if not found.
	Iterator Find(const T& key)
	{
		if (!m_ptrs)
			return End();

		unsigned hashKey = hash(key);
		Node* node = findNode(key, hashKey);
		if (node)
			return Iterator(node);
		else
			return End();
	}

	// Return const iterator to the pair with key, or end iterator if not found.
	ConstIterator Find(const T& key) const
	{
		if (!m_ptrs)
			return End();

		unsigned hashKey = hash(key);
		Node* node = findNode(key, hashKey);
		if (node)
			return ConstIterator(node);
		else
			return End();
	}

	// Return whether contains a pair with key.
	bool Contains(const T& key) const
	{
		if (!m_ptrs)
			return false;

		unsigned hashKey = hash(key);
		return findNode(key, hashKey) != nullptr;
	}

	// Return all the keys.
	Vector<T> Keys() const
	{
		Vector<T> result;
		result.Reserve(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
			result.Push(it->first);
		return result;
	}

	// Return all the values.
	Vector<U> Values() const
	{
		Vector<U> result;
		result.Reserve(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
			result.Push(it->second);
		return result;
	}

	// Return iterator to the first element. Is not the lowest key unless the map has been sorted.
	Iterator Begin() { return Iterator(head()); }
	// Return const iterator to the beginning.
	ConstIterator Begin() const { return ConstIterator(head()); }
	// Return iterator to the end.
	Iterator End() { return Iterator(tail()); }
	// Return const iterator to the end.
	ConstIterator End() const { return ConstIterator(tail()); }
	// Return first keyvalue. Is not the lowest key unless the map has been sorted.
	const T& Front() const { return *Begin(); }
	// Return last keyvalue.
	const T& Back() const { assert(Size()); return *(--End()); }

private:
	// Return head node with correct type.
	Node* head() const { return static_cast<Node*>(HashBase::head()); }
	// Return tail node with correct type.
	Node* tail() const { return static_cast<Node*>(HashBase::tail()); }

	// Reserve the tail node and initial buckets.
	void initialize(size_t numBuckets, size_t numNodes)
	{
		allocateBuckets(0, numBuckets);
		m_allocator = AllocatorInitialize(sizeof(Node), numNodes);
		HashNodeBase* tail = allocateNode();
		setHead(tail);
		setTail(tail);
	}

	// Find a node from the buckets.
	Node* findNode(const T& key, unsigned hashKey) const
	{
		if (!m_ptrs)
			return nullptr;

		Node* node = static_cast<Node*>(ptrs()[hashKey]);
		while (node)
		{
			if (node->pair.first == key)
				return node;
			node = node->Down();
		}

		return nullptr;
	}

	// Find a node and the previous node from the buckets.
	Node* findNode(const T& key, unsigned hashKey, Node*& previous) const
	{
		previous = nullptr;
		if (!m_ptrs)
			return nullptr;

		Node* node = static_cast<Node*>(ptrs()[hashKey]);
		while (node)
		{
			if (node->pair.first == key)
				return node;
			previous = node;
			node = node->Down();
		}

		return nullptr;
	}

	// Insert a key and default value and return either the new or existing node.
	Node* insertNode(const T& key)
	{
		unsigned hashKey = hash(key);

		// If exists, just return the node
		Node* existing = findNode(key, hashKey);
		if (existing)
			return existing;

		Node* newNode = insertNode(tail(), key, U());
		newNode->down = ptrs()[hashKey];
		ptrs()[hashKey] = newNode;

		// Rehash if the maximum load factor has been exceeded
		if (Size() > numBuckets() * MAX_LOAD_FACTOR)
		{
			allocateBuckets(Size(), numBuckets() << 1);
			rehash();
		}

		return newNode;
	}

	// Insert a key and value and return either the new or existing node.
	Node* insertNode(const T& key, const U& value)
	{
		unsigned hashKey = hash(key);

		// If exists, just change the value
		Node* existing = findNode(key, hashKey);
		if (existing)
		{
			existing->pair.second = value;
			return existing;
		}

		Node* newNode = insertNode(tail(), key, value);
		newNode->down = ptrs()[hashKey];
		ptrs()[hashKey] = newNode;

		// Rehash if the maximum load factor has been exceeded
		if (Size() > numBuckets() * MAX_LOAD_FACTOR)
		{
			allocateBuckets(Size(), numBuckets() << 1);
			rehash();
		}

		return newNode;
	}

	// Allocate and insert a node into the list. Return the new node.
	Node* insertNode(Node* dest, const T& key, const U& value)
	{
		// If no pointers yet, allocate with minimum bucket count
		if (!m_ptrs)
		{
			initialize(MIN_BUCKETS, 2);
			dest = head();
		}

		Node* newNode = allocateNode(key, value);
		Node* prev = dest->Prev();
		newNode->next = dest;
		newNode->prev = prev;
		if (prev)
			prev->next = newNode;
		dest->prev = newNode;

		// Reassign the head node if necessary
		if (dest == head())
			setHead(newNode);

		setSize(Size() + 1);

		return newNode;
	}

	// Erase a node from the list. Return pointer to the next element, or to the end if could not erase.
	Node* eraseNode(Node* node)
	{
		// The tail node can not be removed
		if (!node || node == tail())
			return tail();

		Node* prev = node->Prev();
		Node* next = node->Next();
		if (prev)
			prev->next = next;
		next->prev = prev;

		// Reassign the head node if necessary
		if (node == head())
			setHead(next);

		freeNode(node);
		setSize(Size() - 1);

		return next;
	}

	// Allocate a node with optionally specified key and value.
	Node* allocateNode(const T& key = T(), const U& value = U())
	{
		Node* newNode = static_cast<Node*>(AllocatorReserve(m_allocator));
		new(newNode) Node(key, value);
		return newNode;
	}

	// Free a node.
	void freeNode(Node* node)
	{
		(node)->~Node();
		AllocatorFree(m_allocator, node);
	}

	// Rehash the buckets.
	void rehash()
	{
		for (Iterator it = Begin(); it != End(); ++it)
		{
			Node* node = static_cast<Node*>(it.ptr);
			unsigned hashKey = hash(it->first);
			node->down = ptrs()[hashKey];
			ptrs()[hashKey] = node;
		}
	}

	// Compare two nodes.
	static bool compareNodes(Node*& lhs, Node*& rhs) { return lhs->pair.first < rhs->pair.first; }

	// Compute a hash based on the key and the bucket size
	unsigned hash(const T& key) const { return MakeHash(key) & (numBuckets() - 1); }
};