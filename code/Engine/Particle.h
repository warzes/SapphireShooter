#pragma once

class Particle
{
public:
	Particle();

	bool Update(float deltaTime);

	void SetOrigin(const glm::vec3& orig);

	glm::vec3 GetPos() const { return m_position; }

private:
	glm::vec3 m_position;
	glm::vec3 m_origin;
	glm::vec3 m_velocity;
	float m_life;
};