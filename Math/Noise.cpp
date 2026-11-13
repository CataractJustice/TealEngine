#include "Noise.h"
#include <cmath>
#include "randrange.h"
#include "libs/glm/glm.hpp"

	int Noise::seed = 0;

	inline unsigned int Noise::uintHash(unsigned int n) 
	{
		n = (((n + 0x0d29ab87U) ^ 0x8efcc04bU) * 0x8efcc04dU);
		n = (((n + 0xcc10d19cU) ^ 0xb9e228ecU) * 0xeb568a47U); 
		n = (((n + 0x2921a2cdU) ^ 0xe54e872bU) * 0xf7d9a89fU); 
		n = (((n + 0xcbdb5667U) ^ 0x907dd34bU) * 0x83462ccfU); 
		n = (((n + 0x8b9a88d8U) ^ 0x846702d0U) * 0x19525047U); 
		return (((n + 0xb49d18daU) ^ 0xe78ad4bfU) * 0x7273b3d1U);
	}

	inline unsigned int uintHash31(unsigned int x, unsigned int y, unsigned int z) 
	{
		x = (((x + 0x0d29ac61U) ^ 0x8e7aa433U) * 0x5eff4043U);
		y = (((y + 0xcc10d19cU) ^ 0xb9e228e2U) * 0xeb568a47U); 
		z = (((z + 0xf921a2caU) ^ 0xef43472bU) * 0xa7d4d49fU); 
		x = (((x + 0xcbdb5667U) ^ 0x907dd34bU) * 0x83462ccfU); 
		y = (((y + 0x8b9a88d8U) ^ 0x846702d0U) * 0x19525047U); 
		z = (((z + 0x50f7f337U) ^ 0xf17c419aU) * 0xffc85413U); 
		return (Noise::uintHash(x^y)^Noise::uintHash(y^z)^Noise::uintHash(z^x));
	}

	inline unsigned int uintHash41(unsigned int x, unsigned int y, unsigned int z, unsigned int w) 
	{
		x = (((x + 0x0d29ab87U) ^ 0x8efcc04bU) * 0x8efcc04dU);
		y = (((y + 0xcc10d19cU) ^ 0xb9e228ecU) * 0xeb568a47U); 
		z = (((z + 0x2921a2cdU) ^ 0xe54e872bU) * 0xf7d9a89fU); 
		w = (((w + 0x6ef4128dU) ^ 0x5e5e76fbU) * 0xfeee81e5U); 
		x = (((x + 0xcbdb5667U) ^ 0x907dd34bU) * 0x83462ccfU); 
		y = (((y + 0x8b9a88d8U) ^ 0x846702d0U) * 0x19525047U); 
		z = (((z + 0x50f7f337U) ^ 0xf17c419aU) * 0xffc85413U); 
		w = (((w + 0x70835111U) ^ 0x11111111U) * 0x69696969U);
		return Noise::uintHash(x^y^z) ^ Noise::uintHash(y^z^w) ^ Noise::uintHash(z^w^x) ^ Noise::uintHash(w^x^y); 
	}

	float Noise::get(int x, int y, int z)
	{
		return ((float)uintHash31(x,y,z)) / (float)(~((unsigned int)(0)));
	}

	float Noise::get(glm::ivec3 p)
	{
		return get(p.x, p.y, p.z);
	}

	unsigned int Noise::uintGet(int x, int y, int z) 
	{
		return uintHash31(x,y,z);
	}
	
	unsigned int Noise::uintGet(int x, int y, int z, int w) 
	{
		return uintHash41(x,y,z,w);
	}

	unsigned int Noise::uintGet(glm::ivec3 p) 
	{
		return uintHash31(p.x,p.y,p.z);
	}

	void Noise::setSeed(int seed) 
	{
		Noise::seed = seed;
	}