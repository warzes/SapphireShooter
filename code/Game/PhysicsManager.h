#pragma once

class PhysicsManager
{
public:
	static PhysicsManager& Get()
	{
		static PhysicsManager instance;
		return instance;
	}

	float GetGravity() { return m_gravity; }

private:
	PhysicsManager() = default;

	float m_gravity = -90.0f;
};