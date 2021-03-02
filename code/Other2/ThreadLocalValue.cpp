#include "stdafx.h"
#include "ThreadLocalValue.h"
#include "Engine/DebugNew.h"

ThreadLocalValue::ThreadLocalValue()
{
#ifdef _WIN32
	key = TlsAlloc();
	valid = key != TLS_OUT_OF_INDEXES;
#else
	valid = pthread_key_create(&key, 0) == 0;
#endif
}

ThreadLocalValue::~ThreadLocalValue()
{
	if (valid)
	{
#ifdef _WIN32
		TlsFree(key);
#else
		pthread_key_delete(key);
#endif
	}
}

void ThreadLocalValue::SetValue(void* value)
{
	if (valid)
	{
#ifdef _WIN32
		TlsSetValue(key, value);
#else
		pthread_setspecific(key, value);
#endif
	}
}

void* ThreadLocalValue::Value() const
{
	if (valid)
	{
#ifdef _WIN32
		return TlsGetValue(key);
#else
		return pthread_getspecific(key);
#endif
	}
	else
		return nullptr;
}