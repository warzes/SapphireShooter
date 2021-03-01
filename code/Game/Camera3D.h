#pragma once

#if SE_SUPPORT_3D

class Camera3D
{
public:
	void InitCameraPerspective(float fov, float aspectRatio, float near, float far);

	void Update();

	void Rotate(float offsetX, float offsetY);
	void SetPitch(float val) { m_pitch = val; }
	void SetYaw(float val) { m_yaw = val; }

	void MoveForward(float dt);
	void MoveBackward(float dt);
	void StrafeLeft(float dt);
	void StrafeRight(float dt);
	void Rise(float dt);
	void Fall(float dt);
	void MouseMoveUpdate(float dt);

	void SetCameraPos(const glm::vec3 &pos) { m_cameraPos = pos; }	
	void SetCameraSpeed(float cameraSpeed) { m_cameraSpeed = cameraSpeed; }
	void SetCameraSensitivity(float sen) { m_cameraSensitivity = sen; }

	const glm::mat4& GetProjectionMatrix() const { return m_projection; }
	const glm::mat4& GetViewMatrix() const { return m_view; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_projView; }

	glm::vec3& GetCameraPos() { return m_cameraPos; }
	const glm::vec3& GetCameraForward() const { return m_cameraForward; }

	float GetPitch() const { return m_pitch; }
	float GetYaw() const { return m_yaw; }

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_projView;
	glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraForward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);

	float m_fieldOfView = 80.0f;
	float m_cameraSpeed = 20.0f;
	float m_cameraSensitivity = 25.0f;

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_currentPitch = 0.0f;
	float m_currentYaw = 0.0f;
};

#endif // SE_SUPPORT_3D