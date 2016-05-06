
#include "Sector.h"
#include "Environment.h"

#include <SpehsEngine/Exceptions.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/BatchManager.h>


Sector::Sector()
{
	factor = 0;
	localSeed = 0;
	type = MAJOR;
	rawdata = nullptr;
	primaryVisuals = nullptr;
	secondaryVisuals = nullptr;
}
Sector::Sector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer)
{
	buildSector(_parent, _x, _y, _sectorbuild, _layer);
}
Sector::~Sector()
{
	clearData();
	clearVisuals();
}

void Sector::buildSector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer)
{
	clearData();
	clearVisuals();

	if (_parent != nullptr)
	{
		spehs::RNG seedRNG(_parent->localSeed);
		localSeed = seedRNG.mtuirandom();
	}
	else
		localSeed = environment->getEnvRandom()->mtuirandom();

	parent = _parent;
	xInP = _x;
	yInP = _y;
	type = _sectorbuild;
	factor = 8 + 4 * std::pow(2, _layer);
	primaryVisuals = nullptr;
	secondaryVisuals = nullptr;

	//Build data
	switch (type)
	{
	case MAJOR:
		buildData(_parent->getData(_x, _y), MAJOR_SIZE);
		break;

	case MINOR:
		buildData(_parent->getData(_x, _y), MINOR_SIZE);
		break;

	default:
		spehs::exceptions::fatalError("Incorrect SECTORTYPE for sector building!");
		break;
	}
}

SECTORDATA Sector::getData(const uint16_t _x, const uint16_t _y)
{
	//smooth turbulent noise
	//adjust contrast
	//interpolate adjacent sectors even if they have a different parent
	SECTORDATA result;

	return SECTORDATA();
}

void Sector::renderVisuals(const bool _state, const unsigned char _alpha)
{
	if (_state == true)
	{
		if (primaryVisuals != nullptr)
		{
			for (unsigned i = 0; i < primaryVisuals->size(); i++)
			{
				(*primaryVisuals)[i]->setColorAlpha(_alpha);
			}
		}
		else
		{
			primaryVisuals = new std::vector<spehs::Polygon*>;
			if (type == MAJOR)
				primaryVisuals->push_back(spehs::getActiveBatchManager()->createPolygon(4, 0, MAJOR_SIZE, MAJOR_SIZE));
			else
				primaryVisuals->push_back(spehs::getActiveBatchManager()->createPolygon(4, 0, MINOR_SIZE, MINOR_SIZE));

			//Position!
		}
	}
	else
	{
		clearVisuals();
	}
}


//private:
void Sector::buildData(const SECTORDATA _parentData, const uint16_t _size)
{
	spehs::RNG dataRNG(localSeed);
	rawdata = new SECTORDATA*[_size];
	for (uint16_t x = 0; x < _size; x++)
	{
		rawdata[x] = new SECTORDATA[_size];
		for (uint16_t y = 0; y < _size; y++)
		{
			rawdata[x][y].density = dataRNG.mtirandom(0, UINT8_MAX - _parentData.density) + dataRNG.mtirandom(_parentData.density / 2, _parentData.density);
			rawdata[x][y].temperature = dataRNG.mtirandom(0, UINT8_MAX - _parentData.temperature) + dataRNG.mtirandom(_parentData.density / 2, _parentData.density);
			rawdata[x][y].technology = dataRNG.mtirandom(0, UINT8_MAX - _parentData.technology) + dataRNG.mtirandom(_parentData.density / 2, _parentData.density);
		}
	}
}
void Sector::clearData()
{
	if (type == MAJOR)
	{
		for (uint16_t i = 0; i < MAJOR_SIZE; i++)
			delete [] rawdata[i];
		delete [] rawdata;
	}
	else
	{
		for (uint16_t i = 0; i < MINOR_SIZE; i++)
			delete [] rawdata[i];
		delete [] rawdata;
	}
}
void Sector::clearVisuals()
{
	if (primaryVisuals != nullptr)
	{
		for (unsigned i = 0; i < primaryVisuals->size(); i++)
			(*primaryVisuals)[i]->destroy();
		delete primaryVisuals;
		primaryVisuals = nullptr;
	}
	if (secondaryVisuals != nullptr)
	{
		for (unsigned i = 0; i < secondaryVisuals->size(); i++)
			(*secondaryVisuals)[i]->destroy();
		delete secondaryVisuals;
		secondaryVisuals = nullptr;
	}
}