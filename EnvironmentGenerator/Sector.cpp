
#include "Sector.h"
#include "Environment.h"

#include <SpehsEngine/Exceptions.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/BatchManager.h>


#define FACTOR(X) (8 + 4 * std::pow(2, X))


int64_t sectorAllocations;
int64_t sectorDeallocations;


Sector::Sector(const std::shared_ptr<Sector>& _parent, const uint16_t _x, const uint16_t _y, const uint32_t _localSeed, const uint8_t _layer, Environment* _creator)
{
#ifdef _DEBUG
	sectorAllocations++;
#endif

	xInP = _x;
	yInP = _y;
	localSeed = _localSeed;
	layer = _layer;
	data = nullptr;
	parent = _parent;

	//Build data
	spehs::rng::PRNG32 buildRNG(localSeed);
	rawdata.density = buildRNG.uirandom(0, UINT8_MAX - parent->rawdata.density) + buildRNG.uirandom(parent->rawdata.density / 3, parent->rawdata.density);
	rawdata.temperature = buildRNG.uirandom(0, UINT8_MAX - parent->rawdata.temperature) + buildRNG.uirandom(parent->rawdata.temperature / 3, parent->rawdata.temperature);
	rawdata.technology = buildRNG.uirandom(0, UINT8_MAX - parent->rawdata.technology) + buildRNG.uirandom(parent->rawdata.technology / 3, parent->rawdata.technology);

	visual = spehs::getActiveBatchManager()->createPolygon(4, 0, MINOR_SIZE, MINOR_SIZE);
	visual->setColor(rawdata.density, rawdata.temperature, rawdata.technology);
	visual->setPosition(creator->getWorldPosition(std::shared_ptr<Sector>(this)));
}
Sector::~Sector()
{
#ifdef _DEBUG
	sectorDeallocations++;
#endif
}


std::shared_ptr<SECTORDATA> Sector::getData()
{
	if (!data)
	{
		//Build smoothed data
	}
	return data;
}

bool Sector::checkPosition(const uint16_t _x, const uint16_t _y)
{
	return _x == xInP && _y == yInP;
}