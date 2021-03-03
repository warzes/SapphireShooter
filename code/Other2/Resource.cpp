#include "stdafx.h"
#include "Resource.h"
#include "Engine/DebugNew.h"

bool Resource::BeginLoad(Stream&)
{
	return false;
}

bool Resource::EndLoad()
{
	// Resources that do not need access to main-thread critical objects do not need to override this
	return true;
}

bool Resource::Save(Stream&)
{
	SE_LOG_ERROR(("Save not supported for " + TypeName()).CString());
	return false;
}

bool Resource::Load(Stream& source)
{
	bool success = BeginLoad(source);
	if (success)
		success &= EndLoad();

	return success;
}

void Resource::SetName(const String& newName)
{
	name = newName;
	nameHash = StringHash(newName);
}