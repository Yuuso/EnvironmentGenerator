
#pragma once

#include "Sector.h"

#include <SpehsEngine/RNG.h>


struct SectorPosition
{
	Sector major0; //4096x4096 majors
	Sector major1; //4096x4096 majors
	Sector major2; //4096x4096 majors
	Sector major3; //4096x4096 minors

	Sector minor0; //256x256 minors
	Sector minor1; //current
};

class Environment
{
public:
	Environment(const unsigned int &_seed);
	~Environment();

	spehs::RNG* getEnvRandom();
	spehs::RNG_64* getEnvRandom64();

private:
	spehs::RNG* envRandom;
	spehs::RNG_64* envRandom64;
	unsigned int worldSeed;
};
extern Environment* environment;

