
#pragma once

#include <SpehsEngine/RNG.h>

#include <stdint.h>
#include <memory>
#include <vector>


//Amount of sectors per side
#define MAJOR_SIZE 4096
#define MINOR_SIZE 256

//Size of the sectors side
#define MAJOR0_SIZE (UINT64_MAX/MAJOR_SIZE)
#define MAJOR1_SIZE (MAJOR0_SIZE/MAJOR_SIZE)
#define MAJOR2_SIZE (MAJOR1_SIZE/MAJOR_SIZE)
#define MAJOR3_SIZE (MAJOR2_SIZE/MAJOR_SIZE)
#define MINOR0_SIZE (MAJOR3_SIZE/MINOR_SIZE)

namespace spehs{ class WorldPosition; }
class Sector;


/*
	((((2^64) / 4096) / 4096) / 4096) / 4096) / 256 = 256
*/
struct EnvironmentData
{
	std::vector<std::shared_ptr<Sector>> major0; //Divides into majors
	std::vector<std::shared_ptr<Sector>> major1; //Divides into majors
	std::vector<std::shared_ptr<Sector>> major2; //Divides into majors
	std::vector<std::shared_ptr<Sector>> major3; //Divides into minors

	std::vector<std::shared_ptr<Sector>> minor0; //Field Sectors
};

enum SECTORTYPE : uint8_t
{
	MAJOR0 = 0,
	MAJOR1 = 1,
	MAJOR2 = 2,
	MAJOR3 = 3,

	MINOR0 = 4,
};

typedef uint8_t SectorDataType;

struct SECTORDATA
{
	SECTORDATA();
	SectorDataType density;
	SectorDataType temperature;
	SectorDataType technology;
	//Something for asteroid building?
};


class Environment
{
	friend class Sector;
public:
	Environment(const unsigned int &_seed, spehs::WorldPosition* _simulatedPosition);
	~Environment();

	void update();

	//Get SECTORDATA from any part of the world
	std::shared_ptr<SECTORDATA> getData(const spehs::WorldPosition& _position);

	spehs::rng::PRNG32* getEnvRandom();
	spehs::rng::PRNG64* getEnvRandom64();
	const unsigned int getWorldSeed() const;

protected:
	std::shared_ptr<Sector> generateSector(const SECTORTYPE _type, const spehs::WorldPosition& _position);
	std::shared_ptr<Sector> getSectorFrom(const SECTORTYPE _type, const spehs::WorldPosition& _position);

	enum DIRECTION : uint8_t { DOWN, UP, RIGHT, LEFT };
	std::shared_ptr<Sector> getAdjacent(const std::shared_ptr<Sector>& _sector, const DIRECTION _direction);
	spehs::WorldPosition getWorldPosition(const Sector& _sector, const int16_t _xInc = 0, const int16_t _yInc = 0);

	EnvironmentData data;

private:
	const unsigned int worldSeed;
	const SECTORDATA defaultWorldData;
	spehs::WorldPosition* simulatedPosition;
	spehs::rng::PRNG32* envRandom;
	spehs::rng::PRNG64* envRandom64;
};
