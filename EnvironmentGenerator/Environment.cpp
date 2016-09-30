
#include "Environment.h"

#include <SpehsEngine/Console.h>

#include <thread>


const float SECTOR_TIMER(60.0f);



std::mutex envDeleteMutex;//To make sure timers stop correctly if environment is deleted
void sectorTimer(Sector* _sector)
{
	envDeleteMutex.lock();
	if (!Environment::instance)
	{
		envDeleteMutex.unlock();
		return;
	}
	do
	{
		_sector->drop();
		envDeleteMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(SECTOR_TIMER));
		envDeleteMutex.lock();
		if (!Environment::instance)
		{
			envDeleteMutex.unlock();
			return;
		}
	} while (_sector->isActive());
	Environment::instance->deleteSector(_sector);
	envDeleteMutex.unlock();
}


Sector::Sector(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent, EnvironmentData* _parentPixel) : position(_layer, _xInParent, _yInParent), active(true)
{
	if (_parentPixel)
	{
		//generate sector based on parent

		std::thread removeTimer(sectorTimer, this);
	}
	else
	{
		//create 0 sector
	}
}
Sector::~Sector()
{

}

bool Sector::operator==(const Sector &_other)
{
	return position == _other.position;
}

const unsigned char Sector::getLayer() const
{
	return position.layer;
}
SectorPosition Sector::getPosition() const
{
	return position;
}
EnvironmentData Sector::getData(const unsigned char &_x, const unsigned char &_y) const
{
	return data[_x][_y];
}
void Sector::use()
{
	active = true;
}
void Sector::drop()
{
	active = false;
}
bool Sector::is(const unsigned char _layer, const unsigned char _xInParent, const unsigned char _yInParent)
{
	return position.layer == _layer && position.xInParent == _xInParent && position.yInParent == _yInParent;
}
bool Sector::isActive()
{
	activeMutex.lock();
	bool value = active;
	activeMutex.unlock();
	return value;
}


Environment::instance = nullptr;
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

	envDeleteMutex.lock();
	delete instance;
	instance == nullptr;
	envDeleteMutex.unlock();
}
Environment::Environment(const uint64_t &_worldSeed) : worldRandom(_worldSeed), worldSeed(_worldSeed)
{
	//create sector 0
}
Environment::~Environment()
{

}

EnvironmentDataType& Environment::getData(const unsigned char _layer, const unsigned char _x, const unsigned char _y) const
{

}
Sector Environment::getSector(const unsigned char _layer, const unsigned char _x, const unsigned char _y) const
{
	if (_layer == 0)
	{
		return layer0Sector;
	}
	else
	{
		sectorVectorMutex.lock();
		for (unsigned i = 0; i < sectors.size(); i++)
		{
			if (sectors[i].is(_layer, _x, _y))
			{
				sectors[i].use();
				return sectors[i];
			}
		}
		sectorVectorMutex.unlock();
	}
	
	//generate sector
}
void Environment::deleteSector(Sector &_sector)
{
	sectorVectorMutex.lock();
	for (unsigned i = 0; i < sectors.size(); i++)
	{
		if (sectors[i] == _sector)
		{
			sectors.erase(sectors.begin() + i);
		}
	}
	sectorVectorMutex.unlock();
}
