
#include "Environment.h"


Environment::Environment(const unsigned int &_seed)
{
	worldSeed = _seed;
	envRandom = new spehs::RNG(worldSeed);
	envRandom64 = new spehs::RNG_64(worldSeed);
}
Environment::~Environment()
{
}

spehs::RNG* Environment::getEnvRandom()
{
	envRandom->resetRandomSeedEngine();
	return envRandom;
}
spehs::RNG_64* Environment::getEnvRandom64()
{
	envRandom64->resetRandomSeedEngine();
	return envRandom64;
}
