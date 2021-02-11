#pragma once

#include "BaseCamera.h"
#include "Engine.h"

class PerspectiveCamera : public BaseCamera
{
public:
	using BaseCamera::BaseCamera;

	virtual glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(getFov()), (float)GetEngineDescription().window.width / (float)GetEngineDescription().window.height, getNear(), getFar()); }
};