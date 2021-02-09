#pragma once

#include "Camera.h"
#include "Terrain.h"
#include "SpotLight.h"

class Player
{
public:
	static Player& Get() noexcept
	{
		static Player playerInst;
		return playerInst;
	}

	void Init(Camera& cam, const glm::vec3 &initialPosition = glm::vec3(0.0f, 0.0f, 1.0f));
	void Update(Camera& cam, Terrain& terrain, float dt);

	bool IsPlayerWalking();

	SpotLight* GetSpotLight() { return &m_spotLight; }

private:
	Player() = default;
	Player(const Player&) = delete;
	Player(Player&&) = delete;
	void operator=(const Player&) = delete;
	void operator=(Player&&) = delete;

	void processInput(Camera& cam, float dt);

	glm::vec3 m_pos;

	SpotLight m_spotLight;

	float m_jumpHeight = 50.0f;
	float m_upwardSpeed = 0.0f;

	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, CAM_JUMP, TOTAL_CAM_MOVES };
	bool m_camMovements[TOTAL_CAM_MOVES] = { false };
	bool m_walking = false;
	bool m_isInAir = false;
	bool m_jumping = false;
	bool m_sprinting = false;
};