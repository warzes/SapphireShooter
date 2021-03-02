#include "stdafx.h"
#include "StringHash.h"
#include "Engine/DebugNew.h"

const StringHash StringHash::ZERO;

String StringHash::ToString() const
{
	char tempBuffer[CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%08X", value);
	return String(tempBuffer);
}