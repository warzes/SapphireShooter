#include "stdafx.h"
#if SE_SUPPORT_3D
#include "Camera3D.h"
#include "Mouse.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
void Camera3D::InitCameraPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fieldOfView = fov;
	m_projection = glm::perspective(glm::radians(m_fieldOfView), aspectRatio, near, far);
}
//-----------------------------------------------------------------------------
void Camera3D::Update()
{
	if (!::Equals(m_currentPitch, m_pitch) || !::Equals(m_currentYaw, m_yaw))
	{
		if (m_pitch > 89) m_pitch = 89;
		else if (m_pitch < -89) m_pitch = -89;
		if (m_yaw < -360 || m_yaw > 360) m_yaw = 0;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_cameraForward = glm::normalize(front);
		right = glm::normalize(glm::cross(m_cameraForward, m_worldUp));
		m_cameraUpVector = glm::normalize(glm::cross(right, m_cameraForward));
		std::cout << "pitch= " << m_pitch << " yaw=" << m_yaw << std::endl;
		m_currentPitch = m_pitch;
		m_currentYaw = m_yaw;
	}
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraForward, m_cameraUpVector);
	m_projView = GetProjectionMatrix() * GetViewMatrix();
}
//-----------------------------------------------------------------------------
void Camera3D::Rotate(float offsetX, float offsetY)
{
	m_pitch += -offsetY;
	m_yaw += offsetX;
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
	m_cameraPos -= (m_cameraSpeed * dt) * right;
}
//-----------------------------------------------------------------------------
void Camera3D::StrafeRight(float dt)
{
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
void Camera3D::MouseMoveUpdate(float dt)
{
	const float MouseDeltaX = static_cast<float>(Mouse::Get().GetMouseMove().x);
	const float MouseDeltaY = static_cast<float>(Mouse::Get().GetMouseMove().y);
	Rotate(MouseDeltaX * m_cameraSensitivity * dt, MouseDeltaY * m_cameraSensitivity * dt);
}
//-----------------------------------------------------------------------------
#endif // SE_SUPPORT_3D