#pragma once

#include "BaseCamera.h"

class OrthogonalCamera : public BaseCamera
{
public:
	using BaseCamera::BaseCamera;

	virtual glm::mat4 getProjectionMatrix() const { return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, getNear(), getFar()); }
};