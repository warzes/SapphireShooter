#pragma once

class Camera
{
public:
	Camera();

	void UpdateLookAt();

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
	const glm::mat4& GetViewMatrix() const { return m_view; }
	glm::vec3& GetCameraPos() { return m_cameraPos; }
	const glm::vec3& GetCameraForward() const { return m_cameraForward; }
	const glm::vec3& GetCameraUpVector() const { return m_cameraUpVector; }
	const glm::vec3& GetCameraRightVector() const { return m_cameraRight; }
	float GetCameraSpeed() const { return m_cameraSpeed; }
	float GetCameraFOV() const { return m_fieldOfView; }
	float GetCameraSensitivity() const { return m_cameraSensitivity; }

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 m_cameraUpVector;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraForward;
	glm::vec3 m_cameraRight;
	glm::vec2 m_oldMousePos;
	float m_fieldOfView;
	float m_cameraSpeed;
	float m_cameraSensitivity;
};