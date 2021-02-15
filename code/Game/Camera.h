#pragma once

#include "Engine.h"

class Camera
{
public:
	Camera();

	void UpdateLookAt();

	void Rotate(float offsetX, float offsetY);

	void InitCameraPerspective(float fov, float aspectRatio, float near, float far);
	void InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far);

	void MoveForward(float dt);
	void MoveBackward(float dt);
	void StrafeLeft(float dt);
	void StrafeRight(float dt);
	void Rise(float dt);
	void Fall(float dt);
	void MouseUpdate(float dt);

	void SetCameraPos(const glm::vec3 &pos) { m_cameraPos = pos; }
	void SetCameraView(const glm::vec3 &view) { m_cameraForward = view; }
	void SetCameraUpVector(const glm::vec3 &up) { m_cameraUpVector = up; }
	void SetCameraSpeed(float cameraSpeed) { m_cameraSpeed = cameraSpeed; }
	void SetCameraFOV(float fov) { m_fieldOfView = fov; }
	void SetCameraSensitivity(float sen) { m_cameraSensitivity = sen; }

	const glm::mat4& GetProjectionMatrix() const { return m_projection; }
	//const glm::mat4& GetViewMatrix() const { return m_view; }
	glm::mat4 GetViewMatrix() const { return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraForward, m_cameraUpVector); }
	glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }


	glm::vec3& GetCameraPos() { return m_cameraPos; }
	const glm::vec3& GetCameraForward() const { return m_cameraForward; }
	const glm::vec3& GetCameraUpVector() const { return m_cameraUpVector; }
	float GetCameraSpeed() const { return m_cameraSpeed; }
	float GetCameraFOV() const { return m_fieldOfView; }
	float GetCameraSensitivity() const { return m_cameraSensitivity; }

	float GetPitch() const { return pitch; }
	float GetYaw() const { return yaw; }
	void SetPitch(const float& val) { pitch = val; }
	void SetYaw(const float& val) { yaw = val; }

	void updateVectors();

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 m_cameraUpVector;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraForward;
	glm::vec2 m_oldMousePos;
	float m_fieldOfView;
	float m_cameraSpeed;
	float m_cameraSensitivity;

	float pitch = 0.0f;
	float yaw = -90;

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
};