#include "stdafx.h"
#include "PerlinNoise.h"
//-----------------------------------------------------------------------------
// Description: Function that sets up the randomized permutation table
void PerlinNoise::SetSeed(int seed)
{
	// Initialize lower half of array (sequentially) 
	for (int i = 0; i < 128; ++i)
		m_permutation[i] = i;

	// Create random number engine based on random seed
	std::default_random_engine randomNumEngine(seed);

	// Shuffle using the random number engine
	std::shuffle(std::begin(m_permutation), std::begin(m_permutation) + 128, randomNumEngine);

	// Duplicate
	for (int i = 0; i < 128; ++i)
		m_permutation[128 + i] = m_permutation[i];
}
//-----------------------------------------------------------------------------
// Description: Function that calculates the noise
double PerlinNoise::Noise(double x, double y, double z)
{
	double result = 0.0;

	// Find unit cube that contains point 
	const int X = static_cast<int>(std::floor(x)) & 255;
	const int Y = static_cast<int>(std::floor(y)) & 255;
	const int Z = static_cast<int>(std::floor(z)) & 255;

	// Find relative X, Y, Z of point in cube
	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	// Compute the fade curves for each X, Y, Z
	const double u = fade(x);
	const double v = fade(y);
	const double w = fade(z);

	// Hash coordinates of the 8 cube corners
	const int A = m_permutation[X] + Y;
	const int AA = m_permutation[A] + Z;
	const int AB = m_permutation[A + 1] + Z;

	const int B = m_permutation[X + 1] + Y;
	const int BA = m_permutation[B] + Z;
	const int BB = m_permutation[B + 1] + Z;

	// Add blended results from 8 corners of cube
	result = lerp(w, lerp(v, lerp(u, grad(m_permutation[AA], x, y, z),
		grad(m_permutation[BA], x - 1, y, z)),
		lerp(u, grad(m_permutation[AB], x, y - 1, z),
			grad(m_permutation[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(m_permutation[AA + 1], x, y, z - 1),
			grad(m_permutation[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(m_permutation[AB + 1], x, y - 1, z - 1),
				grad(m_permutation[BB + 1], x - 1, y - 1, z - 1))));

	// Return the result 
	return result;
}
//-----------------------------------------------------------------------------
// Description: Function that calculates the octave
double PerlinNoise::Octave(double x, double y, int octaves)
{
	double result = 0.0;
	double amplitude = 1.0;

	for (int i = 0; i < octaves; ++i)
	{
		result += Noise(x, y, 0.0) * amplitude;
		x *= 2.0;
		y *= 2.0;
		amplitude *= 0.5;
	}

	return result;
}
//-----------------------------------------------------------------------------
// Description: Function that calculates the product between a pseudorandom gradient vector and the vector from the input coordinate to the 8 surrounding points in its unit cube
double PerlinNoise::grad(int hash, double x, double y, double z)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;

	default: return 0;
	}
}
//-----------------------------------------------------------------------------