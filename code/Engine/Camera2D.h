#pragma once

class Camera2D
{
public:
	void InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far);

private:
	glm::mat4 m_projection;
};