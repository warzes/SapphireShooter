#include "stdafx.h"
#include "RandomUtils.h"
//-----------------------------------------------------------------------------
RandomUtils::RandomUtils()
{
	m_random.seed(static_cast<unsigned int>(time(NULL)));
}
//-----------------------------------------------------------------------------
int RandomUtils::RandomNumBetween1And100()
{
	std::uniform_int_distribution<int> randomNum(1, 100);
	return randomNum(m_random);
}
//-----------------------------------------------------------------------------
float RandomUtils::RandomNumBetweenTwo(float min, float max)
{
	std::uniform_real_distribution<float> randomNum(min, max);
	return randomNum(m_random);
}
//-----------------------------------------------------------------------------