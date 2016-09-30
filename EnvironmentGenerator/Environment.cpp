
#include "Environment.h"

#include <SpehsEngine/Console.h>

#include <thread>


const int SECTOR_TIMER(60);


bool SectorTimer::end = false;
std::vector<Sector*> SectorTimer::sectors = std::vector<Sector*>();
std::mutex sectorMutex;//To make sure timers stop correctly if environment is deleted
void SectorTimer::sectorTimer()
{
	auto dropAll = [&]()
	{
		for (unsigned i = 0; i < sectors.size(); i++)
		{
			if (sectors[i]->getNumChildren() == 0)
			{
				if (!sectors[i]->drop())
				{
					sectors[i]->deleteFromParent();
					delete sectors[i];
					sectors[i] = sectors.back();
					sectors.pop_back();
					i--;
				}
			}
		}
	};

	sectorMutex.lock();
	do
	{
		dropAll();
		sectorMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(SECTOR_TIMER));
		sectorMutex.lock();
	} while (!end);

	for (unsigned i = 0; i < sectors.size(); i++)
	{
		delete sectors[i];
	}
	sectors.clear();
	sectorMutex.unlock();
}
void SectorTimer::deleteAll()
{
	sectorMutex.lock();
	end = true;
	sectorMutex.unlock();
}
void SectorTimer::add(Sector* _sector)
{
	sectorMutex.lock();
	sectors.push_back(_sector);
	sectorMutex.unlock();
}


/*
	SECTORS
*/
Sector::Sector(const EnvironmentData &_data, Sector* _parent, const LayerPosition &_positionInParent) : data(_data), parent(_parent), positionInParent(_positionInParent), active(true)
{

	SectorTimer::add(this);
}
Sector::~Sector()
{

}

EnvironmentData Sector::getData(const ChunkPosition &_position, const uint8_t _layerCounter)
{
	use();
	return data;
}
void Sector::deleteFromParent()
{
	if (!parent)
	{
		delete Environment::instance->layer0Sectors[positionInParent.x][positionInParent.y];
		Environment::instance->layer0Sectors[positionInParent.x][positionInParent.y] = nullptr;
	}
	else
	{
		delete parent->getAsMinor()->children[positionInParent.x][positionInParent.y];
		parent->getAsMinor()->children[positionInParent.x][positionInParent.y] = nullptr;
		parent->getAsMinor()->numChildren--;
	}
}
void Sector::use()
{
	activeMutex.lock();
	active = true;
	activeMutex.unlock();
}
bool Sector::drop()
{
	activeMutex.lock();
	bool value = active;
	active = false;
	activeMutex.unlock();
	return value;
}

MinorSector::MinorSector(const EnvironmentData &_data, Sector* _parent, const LayerPosition &_positionInParent, const uint8_t _layer) : Sector(_data, _parent, _positionInParent), layer(_layer)
{

}
MinorSector::~MinorSector()
{

}
EnvironmentData MinorSector::getData(const ChunkPosition &_position, const uint8_t _layerCounter)
{
	if (_layerCounter != 0)
	{
		LayerPosition pos;
		_position.getLayerPosition(&pos, layer);
		if (!children[pos.x][pos.y])
		{
			generateSector(_position, pos.x, pos.y);
		}
		return children[pos.x][pos.y]->getData(_position, _layerCounter - 1);
	}
	else
	{
		return data;
	}
}
void MinorSector::generateSector(const ChunkPosition &_position, const uint8_t _x, const uint8_t _y)
{
	numChildren++;
	
	EnvironmentData temp;
	spehs::rng::PRNG64 envRandom(_position.integerX | _position.integerY);

	temp.density = envRandom.uirandom(0, UINT8_MAX) + envRandom.uirandom(parent->data.density / 4, parent->data.density);
	temp.population = envRandom.uirandom(0, UINT8_MAX) + envRandom.uirandom(parent->data.population / 4, parent->data.population);
	temp.technology = envRandom.uirandom(0, UINT8_MAX) + envRandom.uirandom(parent->data.technology / 4, parent->data.technology);
	temp.temperature = envRandom.uirandom(0, UINT8_MAX) + envRandom.uirandom(parent->data.temperature / 4, parent->data.temperature);
	children[_x][_y] = new Sector(temp, nullptr, LayerPosition(_x, _y));
}


/*
	ENVIRONMENT
*/
Environment* Environment::instance = nullptr;
void Environment::create(const uint64_t &_worldSeed)
{
	if (instance)
	{
		spehs::console::error("Environment already created!");
	}

	instance = new Environment(_worldSeed);
}
void Environment::destroy()
{
	if (!instance)
	{
		spehs::console::error("No Environment exists!");
	}

	delete instance;
	instance = nullptr;
}
Environment::Environment(const uint64_t &_worldSeed) : worldRandom(_worldSeed), worldSeed(_worldSeed)
{
	EnvironmentData temp;
	for (unsigned x = 0; x < UINT8_MAX; x++)
	{
		for (unsigned y = 0; y < UINT8_MAX; y++)
		{
			temp.density = worldRandom.uirandom(0, UINT8_MAX);
			temp.population = worldRandom.uirandom(0, UINT8_MAX);
			temp.technology = worldRandom.uirandom(0, UINT8_MAX);
			temp.temperature = worldRandom.uirandom(0, UINT8_MAX);
			layer0Sectors[x][y] = new Sector(temp, nullptr, LayerPosition(x, y));
		}
	}
}
Environment::~Environment()
{
	SectorTimer::deleteAll();
}
EnvironmentData Environment::getData(const ChunkPosition &_position, const uint8_t _layerCounter)
{
	LayerPosition pos;
	_position.getLayerPosition(&pos, 0);
	return layer0Sectors[pos.x][pos.y]->getData(_position, _layerCounter - 1);
}
spehs::rng::PRNG64 Environment::getEnvRandom()
{
	worldRandom.reset();
	return worldRandom;
}