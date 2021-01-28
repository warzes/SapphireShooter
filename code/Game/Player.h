#pragma once

#include "Camera.h"

class Player
{
public:
	static Player& Get() noexcept
	{
		static Player playerInst;
		return playerInst;
	}


private:
	Player() = default;
	Player(const Player&) = delete;
	Player(Player&&) = delete;
	void operator=(const Player&) = delete;
	void operator=(Player&&) = delete;

	glm::vec3 m_pos;
};