
#pragma once

#include "Sector.h"

#include <SpehsEngine/RNG.h>


namespace spehs{ class WorldPosition; }


/*
((((2^64) / 4096) / 4096) / 4096) / 4096) / 256 = 256
*/
struct EnvironmentData
{
	std::vector<Sector> major0; //Divides into majors
	std::vector<Sector> major1; //Divides into majors
	std::vector<Sector> major2; //Divides into majors
	std::vector<Sector> major3; //Divides into minors

	std::vector<Sector> minor0; //Divides into minors
	std::vector<Sector> minor1; //Field Sectors
};

enum SECTORTYPE : uint8_t
{
	MAJOR0,
	MAJOR1,
	MAJOR2,
	MAJOR3,

	MINOR0,
	MINOR1,
};


class Environment
{
public:
	Environment(const unsigned int &_seed);
	~Environment();

	SECTORDATA* getData(const spehs::WorldPosition& _position);

	spehs::RNG* getEnvRandom();
	spehs::RNG_64* getEnvRandom64();
	const unsigned int getWorldSeed() const;

protected:
	Sector* generateSector(const SECTORTYPE _type, const spehs::WorldPosition& _position, const bool _saveSector = false);

	enum DIRECTION : uint8_t { DOWN, UP, RIGHT, LEFT };
	Sector* getAdjacent(Sector* _sector, DIRECTION _direction);

	EnvironmentData data;

private:
	const unsigned int worldSeed;
	spehs::RNG* envRandom;
	spehs::RNG_64* envRandom64;
};
extern Environment* environment;

