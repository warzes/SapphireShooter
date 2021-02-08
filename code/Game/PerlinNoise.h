#pragma once

class PerlinNoise
{
public:
	void SetSeed(int seed);
	double Noise(double x, double y, double z);
	double Octave(double x, double y, int octaves);
	double OctaveNoise(double x, double y, int octaves) { return Octave(x, y, octaves) * 0.5 + 0.5; }

private:
	// Description: Private function that eases coordinate values and smooth final output
	double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

	double lerp(double t, double a, double b) { return a + t * (b - a); }
	double grad(int hash, double x, double y, double z);

	int m_permutation[256];
};