#include "stdafx.h"
#include "Hash.h"
#include "Swap.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
void HashBase::Swap(HashBase& hash)
{
	::Swap(m_ptrs, hash.m_ptrs);
	::Swap(m_allocator, hash.m_allocator);
}
//-----------------------------------------------------------------------------
void HashBase::allocateBuckets(size_t size, size_t numBuckets)
{
	assert(numBuckets >= MIN_BUCKETS);

	// Remember old head & tail pointers
	HashNodeBase* h = head();
	HashNodeBase* t = tail();
	delete[] m_ptrs;

	HashNodeBase** newPtrs = new HashNodeBase * [numBuckets + 4];
	size_t* data = reinterpret_cast<size_t*>(newPtrs);
	data[0] = size;
	data[1] = numBuckets;
	newPtrs[2] = h;
	newPtrs[3] = t;
	m_ptrs = newPtrs;

	resetPtrs();
}
//-----------------------------------------------------------------------------
void HashBase::resetPtrs()
{
	if (m_ptrs)
	{
		size_t num = numBuckets();
		HashNodeBase** data = ptrs();
		for (size_t i = 0; i < num; ++i)
			data[i] = nullptr;
	}
}
//-----------------------------------------------------------------------------
template<> void Swap<HashBase>(HashBase& first, HashBase& second)
{
	first.Swap(second);
}
//-----------------------------------------------------------------------------