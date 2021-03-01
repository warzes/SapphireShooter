#include "stdafx.h"
#if SE_SUPPORT_3D
#include "Camera3D.h"
#include "Mouse.h"
//-----------------------------------------------------------------------------
Camera3D::Camera3D() :
	m_cameraPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_cameraForward(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_cameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_cameraSpeed(20.0f),
	m_fieldOfView(80.0f),
	m_cameraSensitivity(25.0f)
{
}
//-----------------------------------------------------------------------------
void Camera3D::InitCameraPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fieldOfView = fov;
	m_projection = glm::perspective(glm::radians(m_fieldOfView), aspectRatio, near, far);
}
//-----------------------------------------------------------------------------
void Camera3D::InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, up, near, far);
}
//-----------------------------------------------------------------------------
void Camera3D::MoveForward(float dt)
{
	m_cameraPos += (m_cameraSpeed * dt) * m_cameraForward;
}
//-----------------------------------------------------------------------------
void Camera3D::MoveBackward(float dt)
{
	m_cameraPos -= (m_cameraSpeed * dt) * m_cameraForward;
}
//-----------------------------------------------------------------------------
void Camera3D::StrafeLeft(float dt)
{
	glm::vec3 StrafteDirection = glm::cross(m_cameraForward, m_cameraUpVector);
	m_cameraPos -= (m_cameraSpeed * dt) * right;
}
//-----------------------------------------------------------------------------
void Camera3D::StrafeRight(float dt)
{
	glm::vec3 StrafteDirection = glm::cross(m_cameraForward, m_cameraUpVector);
	m_cameraPos += (m_cameraSpeed * dt) * right;
}
//-----------------------------------------------------------------------------
void Camera3D::Rise(float dt)
{
	m_cameraPos += (m_cameraSpeed * dt) * m_cameraUpVector;
}
//-----------------------------------------------------------------------------
void Camera3D::Fall(float dt)
{
	m_cameraPos -= (m_cameraSpeed * dt) * m_cameraUpVector;
}
//-----------------------------------------------------------------------------
void Camera3D::MouseUpdate(float dt)
{
	const float MouseDeltaX = static_cast<float>(Mouse::Get().GetMouseMove().x);
	const float MouseDeltaY = static_cast<float>(Mouse::Get().GetMouseMove().y);
	Rotate(MouseDeltaX * m_cameraSensitivity * dt, MouseDeltaY * m_cameraSensitivity * dt);
}
//-----------------------------------------------------------------------------
void Camera3D::Rotate(float offsetX, float offsetY)
{
	pitch += -offsetY;
	yaw += offsetX;

	if (pitch > 89) pitch = 89;
	else if (pitch < -89) pitch = -89;
	if (yaw > 360) yaw = 0;
	else if (yaw < -360) yaw = 0;

	updateVectors();
}
//-----------------------------------------------------------------------------
void Camera3D::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_cameraForward = glm::normalize(front);
	right = glm::normalize(glm::cross(m_cameraForward, worldUp));
	m_cameraUpVector = glm::normalize(glm::cross(right, m_cameraForward));

	std::cout << "pitch= " << pitch << " yaw=" << yaw << std::endl;
}
//-----------------------------------------------------------------------------
void Camera3D::UpdateLookAt()
{
	updateVectors();
}
//-----------------------------------------------------------------------------
#endif // SE_SUPPORT_3D