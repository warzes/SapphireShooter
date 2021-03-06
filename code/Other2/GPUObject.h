#pragma once

#include "Ptr.h"

class Graphics;

// Base class for objects that allocate GPU resources.
class GPUObject
{
public:
	// Construct. Acquire the %Graphics subsystem if available and register self.
	GPUObject();
	// Destruct. Unregister from the %Graphics subsystem.
	virtual ~GPUObject();

	// Release the GPU resource.
	virtual void Release();
	// Recreate the GPU resource after data loss. Not called on all rendering API's.
	virtual void Recreate();
	// Return whether the contents have been lost due to graphics context having been destroyed.
	virtual bool IsDataLost() const { return dataLost; }

	// Set data lost state. Not needed on all rendering API's.
	void SetDataLost(bool enable) { dataLost = enable; }

protected:
	// Graphics subsystem pointer.
	WeakPtr<Graphics> graphics;

private:
	// Data lost flag.
	bool dataLost;
};