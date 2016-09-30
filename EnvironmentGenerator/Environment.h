
#pragma once

#include "WorldPosition.h"

#include <SpehsEngine\RNG.h>

#include <stdint.h>
#include <vector>
#include <mutex>


typedef uint8_t EnvironmentDataType;


class EnvironmentData
{
public:
	EnvironmentDataType population;
	EnvironmentDataType density;
	EnvironmentDataType technology;
	EnvironmentDataType temperature;
};

class MinorSector;
class Sector
{
	friend class SectorTimer;
	friend class MinorSector;
public:
	Sector(const EnvironmentData &_data, Sector* _parent, const LayerPosition &_positionInParent);
	virtual ~Sector();
	
	virtual EnvironmentData getData(const ChunkPosition &_position, const uint8_t _layerCounter);
	virtual int getNumChildren(){ return 0; }

	virtual MinorSector* getAsMinor(){ return nullptr; }

protected:
	void deleteFromParent();
	void use();
	bool drop(); //Called only from sector timer!

	EnvironmentData data;

private:
	Sector* parent;
	const LayerPosition positionInParent;

	std::mutex activeMutex;
	bool active;
};

class MinorSector : public Sector
{
	friend class Sector;
public:
	MinorSector(const EnvironmentData &_data, Sector* _parent, const LayerPosition &_positionInParent, const uint8_t _layer);
	~MinorSector();

	EnvironmentData getData(const ChunkPosition &_position, const uint8_t _layerCounter);
	int getNumChildren(){ return numChildren; }

	MinorSector* getAsMinor(){ return this; }

protected:
	const uint8_t layer;
	int numChildren;
	Sector*** children;

private:
	void generateSector(const ChunkPosition &_position, const uint8_t _x, const uint8_t _y);
};

class SectorTimer
{
public:
	static void sectorTimer();
	static void deleteAll();
	static void add(Sector* _sector);
private:
	static bool end;
	static std::vector<Sector*> sectors;
};


class Environment
{
	friend class Sector;
public:
	static Environment* instance;
	static void create(const uint64_t &_worldSeed);
	static void destroy();

	EnvironmentData getData(const ChunkPosition &_position, const uint8_t _layerCounter);
	spehs::rng::PRNG64 getEnvRandom();

private:
	Environment(const uint64_t &_worldSeed);
	~Environment();

	uint64_t worldSeed;

	spehs::rng::PRNG64 worldRandom;

	Sector* layer0Sectors[256][256];
};

