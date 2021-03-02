#include "stdafx.h"
#include "String.h"
#include "IntVector2.h"
#include "Engine/DebugNew.h"

const IntVector2 IntVector2::ZERO(0, 0);

bool IntVector2::FromString(const String& str)
{
    return FromString(str.CString());
}

bool IntVector2::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 2)
        return false;

    char* ptr = (char*)str;
    x = strtol(ptr, &ptr, 10);
    y = strtol(ptr, &ptr, 10);

    return true;
}

String IntVector2::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d %d", x, y);
    return String(tempBuffer);
}