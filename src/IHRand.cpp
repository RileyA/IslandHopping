#include "IHRand.h"

namespace IH
{
	const ruint Rand::IHRAND_MAX = RAND_MAX;
	
	Rand::Rand(ruint seed)
	{
		std::cout<<"max: "<<RAND_MAX<<"\n";
		mSeed = (seed == 0 ? time(0) : seed);
	}

	// based on MS implementation of rand()
	// kinda crappy..
	ruint Rand::gen()
	{
		mSeed = mSeed * 1103515245 + 12345;
		return static_cast<ruint>(mSeed >> 16) & IHRAND_MAX;
	}

	ruint Rand::gen(int min, int max)
	{
		return min < max ?
			min + gen() % (max - min + 1):
			min + gen() % (max - min + 1);
	}

	float Rand::genFloat(float min, float max)
	{
		return (min < max) ?
			gen()/(1.f + IHRAND_MAX) * (max - min) + min:
			gen()/(1.f + IHRAND_MAX) * (min - max) + max;
	}
	
	char Rand::genChar()
	{
		return static_cast<char>(gen(0,127));
	}
}
