#include "stdafx.h"
#include "Camera2D.h"

//-----------------------------------------------------------------------------
void Camera2D::InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, up, near, far);
}
//-----------------------------------------------------------------------------