
#pragma once

#include <SpehsEngine\RNG.h>

#include <stdint.h>
#include <vector>
#include <mutex>

typedef unsigned char EnvironmentDataType;


class SectorPosition
{
public:
	SectorPosition(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent)
		: layer(_layer), xInParent(_xInParent), yInParent(_yInParent) {}

	bool operator==(const SectorPosition &_other)
	{
		return layer == _other.layer && xInParent == _other.xInParent && yInParent == _other.yInParent;
	}
	
	const unsigned char layer;
	const unsigned char xInParent;
	const unsigned char yInParent;
};

/*
	Sector layers:
	0 is the largers size layer (only one of these exist)
	7 is the smallest size with one element(pixel) being one chunk in size
*/
class Sector
{
	friend class Environment;
public:
	Sector(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent, EnvironmentData* _parentPixel);
	~Sector();

	bool operator==(const Sector &_other);

	const unsigned char getLayer() const;
	SectorPosition getPosition() const;
	EnvironmentData getData(const unsigned char &_x, const unsigned char &_y) const;

protected:
	void use();
	void drop();
	bool is(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent);
	bool isActive();

private:
	std::mutex activeMutex;
	bool active;

	SectorPosition position;
	EnvironmentData data[256][256];
};

class EnvironmentData
{
public:
	EnvironmentDataType population;
	EnvironmentDataType density;
	EnvironmentDataType technology;
	EnvironmentDataType temperature;
};

class Environment
{
public:
	static Environment* instance;
	static void create(const uint64_t &_worldSeed);
	static void destroy();

	EnvironmentDataType& getData(const unsigned char _layer, const unsigned char _x, const unsigned char _y) const;

	//Layers from 0-7
	//xy from parent sector (don't matter for 0)
	Sector getSector(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent) const;

protected:
	void deleteSector(Sector &_sector);

private:
	Environment(const uint64_t &_worldSeed);
	~Environment();

	uint64_t worldSeed;

	spehs::rng::PRNG64 worldRandom;

	std::mutex sectorVectorMutex;
	std::vector<Sector> sectors;
	Sector layer0Sector;
};

