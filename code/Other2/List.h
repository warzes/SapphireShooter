#pragma once

#include "Allocator.h"
#include "Swap.h"

// Doubly-linked list node base class.
struct ListNodeBase
{
	ListNodeBase* prev = nullptr;
	ListNodeBase* next = nullptr;
};

// Doubly-linked list iterator base class.
struct ListIteratorBase
{
	ListIteratorBase() = default;
	explicit ListIteratorBase(ListNodeBase* ptr_) : ptr(ptr_) {}

	// Test for equality with another iterator.
	bool operator==(const ListIteratorBase& rhs) const { return ptr == rhs.ptr; }
	// Test for inequality with another iterator.
	bool operator!=(const ListIteratorBase& rhs) const { return ptr != rhs.ptr; }

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
	ListNodeBase* ptr  =nullptr;
};

// Doubly-linked list base class.
class ListBase
{
public:
	~ListBase()
	{
		delete[] m_ptrs;
	}

	// Swap with another linked list.
	void Swap(ListBase& list)
	{
		::Swap(m_ptrs, list.m_ptrs);
		::Swap(m_allocator, list.m_allocator);
	}

	// Return number of elements.
	size_t Size() const { return m_ptrs ? (reinterpret_cast<size_t*>(m_ptrs))[0] : 0; }
	// Return whether has no elements.
	bool IsEmpty() const { return Size() == 0; }

protected:
	// Allocate head & tail pointers + room for size variable.
	void allocatePtrs();
	// Set new size.
	void setSize(size_t size) { reinterpret_cast<size_t*>(m_ptrs)[0] = size; }
	// Set new head node.
	void setHead(ListNodeBase* head) { m_ptrs[1] = head; }
	// Set new tail node.
	void setTail(ListNodeBase* tail) { m_ptrs[2] = tail; }

	// Return list head node.
	ListNodeBase* head() const { return m_ptrs ? m_ptrs[1] : nullptr; }
	// Return list tail node.
	ListNodeBase* tail() const { return m_ptrs ? m_ptrs[2] : nullptr; }

	// Head & tail pointers and list size.
	ListNodeBase** m_ptrs = nullptr;
	// Node allocator.
	AllocatorBlock* m_allocator = nullptr;
};

// Doubly-linked list template class. Elements can generally be assumed to be in non-continuous memory.
template <class T> 
class List : public ListBase
{
public:
	// List node.
	struct Node : public ListNodeBase
	{
		Node() = default;
		Node(const T& value_) : value(value_) {}
		
		// Return next node.
		Node* Next() const { return static_cast<Node*>(next); }
		// Return previous node.
		Node* Prev() { return static_cast<Node*>(prev); }

		// Node value.
		T value;
	};

	// List iterator.
	struct Iterator : public ListIteratorBase
	{
		Iterator() = default;
		explicit Iterator(Node* ptr_) : ListIteratorBase(ptr_) {}

		// Preincrement the pointer.
		Iterator& operator++() { GotoNext(); return *this; }
		// Postincrement the pointer.
		Iterator operator++(int) { Iterator it = *this; GotoNext(); return it; }
		// Predecrement the pointer.
		Iterator& operator--() { GotoPrev(); return *this; }
		// Postdecrement the pointer.
		Iterator operator--(int) { Iterator it = *this; GotoPrev(); return it; }

		// Point to the node value.
		T* operator->() const { return &(static_cast<Node*>(ptr))->value; }
		// Dereference the node value.
		T& operator*() const { return (static_cast<Node*>(ptr))->value; }
	};

	// List const iterator.
	struct ConstIterator : public ListIteratorBase
	{
		ConstIterator() = default;
		explicit ConstIterator(Node* ptr_) : ListIteratorBase(ptr_) {}
		ConstIterator(const Iterator& it) : ListIteratorBase(it.ptr) {}

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

		// Point to the node value.
		const T* operator->() const { return &(static_cast<Node*>(ptr))->value; }
		// Dereference the node value.
		const T& operator*() const { return (static_cast<Node*>(ptr))->value; }
	};

	List() = default;

	List(const List<T>& list)
	{
		// Reserve the tail node + initial capacity according to the list's size
		Initialize(list.Size() + 1);
		*this = list;
	}

	~List()
	{
		if (m_ptrs && m_allocator)
		{
			Clear();
			FreeNode(tail());
			AllocatorUninitialize(m_allocator);
		}
	}

	// Assign from another list.
	List& operator=(const List<T>& rhs)
	{
		if (&rhs != this)
		{
			Clear();
			Insert(End(), rhs);
		}
		return *this;
	}

	// Add-assign an element.
	List& operator+=(const T& rhs)
	{
		Push(rhs);
		return *this;
	}

	// Add-assign a list.
	List& operator+=(const List<T>& rhs)
	{
		Insert(End(), rhs);
		return *this;
	}

	// Test for equality with another list.
	bool operator==(const List<T>& rhs) const
	{
		if (rhs.Size() != Size())
			return false;

		ConstIterator it = Begin();
		ConstIterator rhsIt = rhs.Begin();
		while (it != End())
		{
			if (*it != *rhsIt)
				return false;
			++it;
			++rhsIt;
		}

		return true;
	}

	// Test for inequality with another list.
	bool operator!=(const List<T>& rhs) const { return !(*this == rhs); }

	// Insert an element to the end.
	void Push(const T& value) { InsertNode(tail(), value); }
	// Insert an element to the beginning.
	void PushFront(const T& value) { InsertNode(head(), value); }
	// Insert an element at position.
	void Insert(const Iterator& dest, const T& value) { insertNode(static_cast<Node*>(dest.ptr_), value); }

	// Insert a list at position.
	void Insert(const Iterator& dest, const List<T>& list)
	{
		Node* destNode = static_cast<Node*>(dest.ptr);
		for (ConstIterator it = list.Begin(); it != list.End(); ++it)
			destNode = insertNode(destNode, *it)->Next();
	}

	// Insert elements by iterators.
	void Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
	{
		Node* destNode = static_cast<Node*>(dest.ptr);
		ConstIterator it = start;
		while (it != end)
			destNode = InsertNode(destNode, *it++)->Next();
	}

	// Insert elements.
	void Insert(const Iterator& dest, const T* start, const T* end)
	{
		Node* destNode = static_cast<Node*>(dest.ptr);
		const T* ptr = start;
		while (ptr != end)
			destNode = InsertNode(destNode, *ptr++)->Next();
	}

	// Erase the last element.
	void Pop()
	{
		if (Size())
			Erase(--End());
	}

	// Erase the first element.
	void PopFront()
	{
		if (Size())
			Erase(Begin());
	}

	// Erase an element by iterator. Return iterator to the next element.
	Iterator Erase(Iterator it)
	{
		return Iterator(EraseNode(static_cast<Node*>(it.ptr)));
	}

	// Erase a range by iterators. Return an iterator to the next element.
	Iterator Erase(const Iterator& start, const Iterator& end)
	{
		Iterator it = start;
		while (it != end)
			it = eraseNode(static_cast<Node*>(it.ptr));

		return it;
	}

	// Clear the list.
	void Clear()
	{
		if (Size())
		{
			for (Iterator it = Begin(); it != End(); )
				freeNode(static_cast<Node*>(it++.ptr));

			Node* tail = tail();
			tail->prev = nullptr;
			setHead(tail);
			setSize(0);
		}
	}

	// Resize the list by removing or adding items at the end.
	void Resize(size_t newSize)
	{
		while (Size() > newSize)
			Pop();

		while (Size() < newSize)
			insertNode(tail(), T());
	}

	// Return iterator to value, or to the end if not found.
	Iterator Find(const T& value)
	{
		Iterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}

	// Return const iterator to value, or to the end if not found.
	ConstIterator Find(const T& value) const
	{
		ConstIterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}

	// Return whether contains a specific value.
	bool Contains(const T& value) const { return Find(value) != End(); }
	// Return iterator to the first element.
	Iterator Begin() { return Iterator(head()); }
	// Return const iterator to the first element.
	ConstIterator Begin() const { return ConstIterator(head()); }
	// Return iterator to the end.
	Iterator End() { return Iterator(tail()); }
	// Return const iterator to the end.
	ConstIterator End() const { return ConstIterator(tail()); }
	// Return first element.
	T& Front() { return *Begin(); }
	// Return const first element.
	const T& Front() const { return *Begin(); }
	// Return last element.
	T& Back() { return *(--End()); }
	// Return const last element.
	const T& Back() const { return *(--End()); }

private:
	// Return the head node.
	Node* head() const { return static_cast<Node*>(ListBase::head()); }
	// Return the tail node.
	Node* tail() const { return static_cast<Node*>(ListBase::tail()); }

	// Reserve the tail node and initial node capacity.
	void initialize(size_t numNodes)
	{
		allocatePtrs();
		m_allocator = AllocatorInitialize(sizeof(Node), numNodes);
		Node* tail = allocateNode();
		SetHead(tail);
		SetTail(tail);
	}

	// Allocate and insert a node into the list. Return the new node.
	Node* insertNode(Node* dest, const T& value)
	{
		if (!dest)
		{
			// If not initialized yet, the only possibility is to insert before the tail
			if (!m_ptrs)
			{
				initialize(1);
				dest = tail();
			}
			else
				return nullptr;
		}

		Node* newNode = allocateNode(value);
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

	// Erase and free a node. Return pointer to the next node, or to the end if could not erase.
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

	// Reserve a node with optionally specified value.
	Node* allocateNode(const T& value = T())
	{
		Node* newNode = static_cast<Node*>(AllocatorGet(m_allocator));
		new(newNode) Node(value);
		return newNode;
	}

	// Free a node.
	void freeNode(Node* node)
	{
		(node)->~Node();
		allocatorFree(m_allocator, node);
	}
};