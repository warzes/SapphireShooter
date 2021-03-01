#include "stdafx.h"
#include "Camera.h"
#include "Mouse.h"
//-----------------------------------------------------------------------------
Camera::Camera() :
	m_cameraPos(glm::vec3(0.0f, 0.0f, 1.0f)),
	m_cameraForward(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_cameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_cameraSpeed(20.0f),
	m_fieldOfView(80.0f),
	m_cameraSensitivity(25.0f)
{
}
//-----------------------------------------------------------------------------
void Camera::InitCameraPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fieldOfView = fov;
	m_projection = glm::perspective(glm::radians(m_fieldOfView), aspectRatio, near, far);
}
//-----------------------------------------------------------------------------
void Camera::InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, up, near, far);
}
//-----------------------------------------------------------------------------
void Camera::MoveForward(float dt)
{
	m_cameraPos += (m_cameraSpeed * dt) * m_cameraForward;
}
//-----------------------------------------------------------------------------
void Camera::MoveBackward(float dt)
{
	m_cameraPos -= (m_cameraSpeed * dt) * m_cameraForward;
}
//-----------------------------------------------------------------------------
void Camera::StrafeLeft(float dt)
{
	glm::vec3 StrafteDirection = glm::cross(m_cameraForward, m_cameraUpVector);
	m_cameraPos -= (m_cameraSpeed * dt) * right;
}
//-----------------------------------------------------------------------------
void Camera::StrafeRight(float dt)
{
	glm::vec3 StrafteDirection = glm::cross(m_cameraForward, m_cameraUpVector);
	m_cameraPos += (m_cameraSpeed * dt) * right;
}
//-----------------------------------------------------------------------------
void Camera::Rise(float dt)
{
	m_cameraPos += (m_cameraSpeed * dt) * m_cameraUpVector;
}
//-----------------------------------------------------------------------------
void Camera::Fall(float dt)
{
	m_cameraPos -= (m_cameraSpeed * dt) * m_cameraUpVector;
}
//-----------------------------------------------------------------------------
void Camera::MouseUpdate(float dt)
{
	float MouseDeltaX = static_cast<float>(Mouse::Get().GetMouseMove().x);
	float MouseDeltaY = static_cast<float>(Mouse::Get().GetMouseMove().y);

	Rotate(MouseDeltaX * dt, MouseDeltaY * dt);
}
//-----------------------------------------------------------------------------
void Camera::Rotate(float offsetX, float offsetY)
{
	yaw += offsetX * m_cameraSensitivity;
	pitch += -offsetY * m_cameraSensitivity;

	if (pitch > 89) pitch = 89;
	else if (pitch < -89) pitch = -89;
	updateVectors();
}
//-----------------------------------------------------------------------------
void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_cameraForward = glm::normalize(front);
	right = glm::normalize(glm::cross(m_cameraForward, worldUp));
	m_cameraUpVector = glm::normalize(glm::cross(right, m_cameraForward));
}
//-----------------------------------------------------------------------------
void Camera::UpdateLookAt()
{
	updateVectors();
	//m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraForward, m_cameraUpVector);
}
//-----------------------------------------------------------------------------