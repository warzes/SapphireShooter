#include "stdafx.h"
#include "Camera.h"
#include "Mouse2.h"
//-----------------------------------------------------------------------------
Camera::Camera() :
	m_cameraPos(glm::vec3(0.0f, 0.0f, 1.0f)),
	m_cameraForward(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_cameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_cameraSpeed(20.0f),
	m_fieldOfView(80.0f),
	m_cameraSensitivity(7.0f)
{
	m_cameraRight = glm::normalize(glm::cross(m_cameraUpVector, -m_cameraForward));
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
	m_cameraPos -= (m_cameraSpeed * dt) * StrafteDirection;
}
//-----------------------------------------------------------------------------
void Camera::StrafeRight(float dt)
{
	glm::vec3 StrafteDirection = glm::cross(m_cameraForward, m_cameraUpVector);
	m_cameraPos += (m_cameraSpeed * dt) * StrafteDirection;
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
void Camera::MouseUpdate(const glm::vec2& newMousePos, float dt)
{
	float MouseDeltaX = static_cast<float>(Mouse2::Get().MouseMove().x);
	float MouseDeltaY = static_cast<float>(Mouse2::Get().MouseMove().y);

	glm::vec3 RotateAround = glm::cross(m_cameraForward, m_cameraUpVector);

	m_cameraForward = glm::mat3(glm::rotate(glm::radians(-MouseDeltaX) * m_cameraSensitivity * dt, m_cameraUpVector)) * m_cameraForward;
	m_cameraForward = glm::mat3(glm::rotate(glm::radians(-MouseDeltaY) * m_cameraSensitivity * dt, RotateAround)) * m_cameraForward;
}
//-----------------------------------------------------------------------------
void Camera::UpdateLookAt()
{
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraForward, m_cameraUpVector);
}
//-----------------------------------------------------------------------------