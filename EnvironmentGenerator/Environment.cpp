
#include "Environment.h"

#include <SpehsEngine/WorldPosition.h>


Environment* environment;
Environment::Environment(const unsigned int &_seed) : worldSeed(_seed)
{
	envRandom = new spehs::RNG(worldSeed);
	envRandom64 = new spehs::RNG_64(worldSeed);
}
Environment::~Environment()
{
}

SECTORDATA* Environment::getData(const spehs::WorldPosition& _position)
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
const unsigned int Environment::getWorldSeed() const
{
	return worldSeed;
}
