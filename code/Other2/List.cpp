#include "stdafx.h"
#include "List.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
template<> void Swap<ListBase>(ListBase& first, ListBase& second)
{
	first.Swap(second);
}
//-----------------------------------------------------------------------------
void ListBase::allocatePtrs()
{
	m_ptrs = new ListNodeBase * [3];

	setSize(0);
	m_ptrs[1] = nullptr;
	m_ptrs[2] = nullptr;
}
//-----------------------------------------------------------------------------