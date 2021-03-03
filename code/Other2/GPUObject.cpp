#include "stdafx.h"
#include "GPUObject.h"
#include "Graphics.h"
#include "Engine/DebugNew.h"

GPUObject::GPUObject() :
	dataLost(false)
{
	graphics = Object::Subsystem<Graphics>();
	if (graphics)
		graphics->AddGPUObject(this);
}

GPUObject::~GPUObject()
{
	if (graphics)
		graphics->RemoveGPUObject(this);
}

void GPUObject::Release()
{
}

void GPUObject::Recreate()
{
}