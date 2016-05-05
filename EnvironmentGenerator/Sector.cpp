
#include "Sector.h"
#include "Environment.h"

#include <SpehsEngine/Exceptions.h>
#include <SpehsEngine/RNG.h>


Sector::Sector(const SECTORDATA _parentData, const SECTORTYPE _sectorbuild, const uint8_t _layer)
{
	type = _sectorbuild;
	factor = 8 + 4 * std::pow(2, _layer);

	//Build data
	switch (type)
	{
	case MAJOR:
		buildData(_parentData, MAJOR_SIZE);
		break;

	case MINOR:
		buildData(_parentData, MINOR_SIZE);
		break;

	default:
		spehs::exceptions::fatalError("Incorrect SECTORTYPE for sector building!");
		break;
	}
}
Sector::~Sector()
{
	if (type == MAJOR)
	{
		for (uint16_t i = 0; i < MAJOR_SIZE; i++)
		{
			delete [] rawdata[i];
		}
		delete [] rawdata;
	}
	else
	{
		for (uint16_t i = 0; i < MINOR_SIZE; i++)
		{
			delete [] rawdata[i];
		}
		delete [] rawdata;
	}
}

SECTORDATA Sector::getData(const uint16_t _x, const uint16_t _y)
{
	//smooth turbulent noise
	//adjust contrast
	//interpolate adjacent sectors even if they have a different parent
	return SECTORDATA();
}


//private:
void Sector::buildData(const SECTORDATA _parentData, const uint16_t _size)
{
	spehs::RNG dataRNG(environment->getEnvRandom()->mtuirandom());/*?*/
	rawdata = new SECTORDATA*[_size];
	for (uint16_t x = 0; x < _size; x++)
	{
		rawdata[x] = new SECTORDATA[_size];
		for (uint16_t y = 0; y < _size; y++)
		{
			rawdata[x][y].density = dataRNG.mtirandom(0, UINT8_MAX);/*?*/
			rawdata[x][y].temperature = dataRNG.mtirandom(0, UINT8_MAX);/*?*/
			rawdata[x][y].technology = dataRNG.mtirandom(0, UINT8_MAX);/*?*/
		}
	}
}