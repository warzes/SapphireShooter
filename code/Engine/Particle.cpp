#include "stdafx.h"
#include "Particle.h"
#include "RandomUtils.h"
//-----------------------------------------------------------------------------
Particle::Particle() 
	: m_position(0.0f, 0.0f, 0.0f)
	, m_life(RandomUtils::Get().RandomNumBetweenTwo(0.5f, 1.5f))
{
	m_velocity = glm::vec3(
		RandomUtils::Get().RandomNumBetweenTwo(-1.0f, 1.0f), 
		RandomUtils::Get().RandomNumBetweenTwo(1.0f, 5.0f), 
		RandomUtils::Get().RandomNumBetweenTwo(-2.0f, 2.0f));
	m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
}
//-----------------------------------------------------------------------------
bool Particle::Update(float deltaTime)
{
	m_life -= deltaTime;
	if (m_life > 0.0f)
	{
		m_position += m_velocity * deltaTime;
	}
	else
	{
		m_position = m_origin;
		m_velocity = glm::vec3(
			RandomUtils::Get().RandomNumBetweenTwo(-2.0f, 2.0f), 
			RandomUtils::Get().RandomNumBetweenTwo(5.0f, 10.0f), 
			RandomUtils::Get().RandomNumBetweenTwo(-4.0f, 4.0f));
		m_life = RandomUtils::Get().RandomNumBetweenTwo(1.5f, 2.5f);
	}

	return true;
}
//-----------------------------------------------------------------------------
void Particle::SetOrigin(const glm::vec3& orig)
{
	m_origin = orig;
}
//-----------------------------------------------------------------------------