#pragma once

#include "InterfaceCamera.h"

class BaseCamera : public InterfaceCamera
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	float pitch;
	float yaw;
	float speed;
	float sensitivity;
	float fov;
	float far;
	float near;


public:
	BaseCamera(
		const glm::vec3& pPosition = cameraPosition,
		const glm::vec3& pDirection = cameraDirection,
		const float& pPitch = cameraPitch,
		const float& pYaw = cameraYaw,
		const float& pSpeed = cameraSpeed,
		const float& pSensitivity = cameraSensitivity,
		const float& pFov = cameraFov,
		const float& pNear = cameraNear,
		const float& pFar = cameraFar
	) : position(pPosition), direction(pDirection),
		pitch(pPitch), yaw(pYaw), speed(pSpeed), sensitivity(pSensitivity),
		fov(pFov), near(pNear), far(pFar) {
		updateVectors();
	}

	virtual void updateVectors();
	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveTop();
	virtual void moveBottom();
	virtual void rotate(const float& offsetX, const float& offsetY);
	virtual glm::mat4 getViewMatrix() const { return glm::lookAt(position, position + direction, up); }
	virtual glm::mat4 getProjectionMatrix() const = 0;

	virtual void setPosition(const glm::vec3& vec) { position = vec; }
	virtual void setDirection(const glm::vec3& vec) { direction = vec; }
	virtual void setPitch(const float& val) { pitch = val; }
	virtual void setYaw(const float& val) { yaw = val; }
	virtual void setSpeed(const float& val) { speed = val; }
	virtual void setFov(const float& val) { fov = val; }
	virtual void setFar(const float& val) { far = val; }
	virtual void setNear(const float& val) { near = val; }
	virtual void setSensitivity(const float& val) { sensitivity = val; }

	virtual glm::vec3 getPosition() const { return position; }
	virtual glm::vec3 getDirection() const { return direction; }
	virtual float getPitch() const { return pitch; }
	virtual float getYaw() const { return yaw; }
	virtual float getSpeed() const { return speed; }
	virtual float getFov() const { return fov; }
	virtual float getNear() const { return near; }
	virtual float getFar() const { return far; }
};