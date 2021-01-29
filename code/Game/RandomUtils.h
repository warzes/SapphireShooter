#pragma once

class RandomUtils
{
public:
	static RandomUtils& Get()
	{
		static RandomUtils instance;
		return instance;
	}

	int RandomNumBetween1And100();
	float RandomNumBetweenTwo(float min, float max);

private:
	RandomUtils();
	RandomUtils(const RandomUtils&) = delete;
	void operator=(const RandomUtils&) = delete;

	std::mt19937 m_random;
};