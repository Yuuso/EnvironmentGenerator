
#pragma once

#include "Sector.h"

#include <SpehsEngine/RNG.h>


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

