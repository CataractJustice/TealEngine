#pragma once

class Noise
{
private:
	static int seed;
	int w, h, l, length;

public:
	static float get(int x, int y = 1, int z = 1);
	static void setSeed(int seed);
};