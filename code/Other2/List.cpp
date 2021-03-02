#include "stdafx.h"
#include "List.h"
#include "Engine/DebugNew.h"

template<> void Swap<ListBase>(ListBase& first, ListBase& second)
{
	first.Swap(second);
}

void ListBase::AllocatePtrs()
{
	ptrs = new ListNodeBase * [3];

	SetSize(0);
	ptrs[1] = nullptr;
	ptrs[2] = nullptr;
}