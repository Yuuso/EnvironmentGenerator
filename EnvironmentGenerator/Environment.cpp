
#include "Environment.h"

#include <SpehsEngine/WorldPosition.h>
#include <SpehsEngine/Exceptions.h>


#define POS_MAJOR0(P) (P / MAJOR0_SIZE)
#define POS_MAJOR1(P) ((P - (POS_MAJOR0(P) * MAJOR0_SIZE)) / MAJOR1_SIZE)
#define POS_MAJOR2(P) ((P - (POS_MAJOR1(P) * MAJOR1_SIZE)) / MAJOR2_SIZE)
#define POS_MAJOR3(P) ((P - (POS_MAJOR2(P) * MAJOR2_SIZE)) / MAJOR3_SIZE)
#define POS_MINOR0(P) ((P - (POS_MAJOR3(P) * MAJOR3_SIZE)) / MINOR0_SIZE)
#define POS_MINOR1(P) ((P - (POS_MINOR0(P) * MINOR0_SIZE)) / MINOR1_SIZE)


#define DEFAULT_DENSITY 50
#define DEFAULT_TEMPERATURE 25
#define DEFAULT_TECHNOLOGY 50


Environment::Environment(const unsigned int &_seed, spehs::WorldPosition* _simulatedPosition) : worldSeed(_seed), defaultWorldData({ DEFAULT_DENSITY, DEFAULT_TEMPERATURE, DEFAULT_TECHNOLOGY })
{
	simulatedPosition = _simulatedPosition;
	envRandom = new spehs::rng::PRNG32(worldSeed);
	envRandom64 = new spehs::rng::PRNG64(worldSeed);

	data.minor1.resize(1);
}
Environment::~Environment()
{
	delete envRandom;
	delete envRandom64;
}

void Environment::update()
{
	data.minor1[0] = getSectorFrom(MINOR1, *simulatedPosition);
}

std::shared_ptr<SECTORDATA> Environment::getData(const spehs::WorldPosition& _position)
{
	//Returns a shared_ptr, which means that environment doesn't have to worry about data that the game uses. (Sectors may be deleted)
	return getSectorFrom(MINOR1, _position)->getData();
}

spehs::rng::PRNG32* Environment::getEnvRandom()
{
	envRandom->reset();
	return envRandom;
}
spehs::rng::PRNG64* Environment::getEnvRandom64()
{
	envRandom64->reset();
	return envRandom64;
}
const unsigned int Environment::getWorldSeed() const
{
	return worldSeed;
}


//Protected:
std::shared_ptr<Sector> Environment::generateSector(const SECTORTYPE _type, const spehs::WorldPosition& _position)
{
	std::shared_ptr<Sector> parent;
	switch (_type)
	{
	case MAJOR0:
		parent = nullptr;
		envRandom->reset();
		int discard = POS_MAJOR0(_position.integerY) * MAJOR_SIZE + POS_MAJOR0(_position.integerX);
		if (discard < 0)
			discard = (MAJOR_SIZE * MAJOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MAJOR0(_position.integerX), POS_MAJOR0(_position.integerY), envRandom->uirandom(), 0, this));
		break;

	case MAJOR1:
		parent = getSectorFrom(MAJOR0, _position);
		envRandom->reset();
		int discard = POS_MAJOR1(_position.integerY) * MAJOR_SIZE + POS_MAJOR1(_position.integerX);
		if (discard < 0)
			discard = (MAJOR_SIZE * MAJOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MAJOR1(_position.integerX), POS_MAJOR1(_position.integerY), envRandom->uirandom(), 1, this));
		break;

	case MAJOR2:
		parent = getSectorFrom(MAJOR1, _position);
		envRandom->reset();
		int discard = POS_MAJOR2(_position.integerY) * MAJOR_SIZE + POS_MAJOR2(_position.integerX);
		if (discard < 0)
			discard = (MAJOR_SIZE * MAJOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MAJOR2(_position.integerX), POS_MAJOR2(_position.integerY), envRandom->uirandom(), 2, this));
		break;

	case MAJOR3:
		parent = getSectorFrom(MAJOR2, _position);
		envRandom->reset();
		int discard = POS_MAJOR3(_position.integerY) * MAJOR_SIZE + POS_MAJOR3(_position.integerX);
		if (discard < 0)
			discard = (MAJOR_SIZE * MAJOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MAJOR3(_position.integerX), POS_MAJOR3(_position.integerY), envRandom->uirandom(), 3, this));
		break;

	case MINOR0:
		parent = getSectorFrom(MAJOR3, _position);
		envRandom->reset();
		int discard = POS_MINOR0(_position.integerY) * MINOR_SIZE + POS_MINOR0(_position.integerX);
		if (discard < 0)
			discard = (MINOR_SIZE * MINOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MINOR0(_position.integerX), POS_MINOR0(_position.integerY), envRandom->uirandom(), 4, this));
		break;

	case MINOR1:
		parent = getSectorFrom(MINOR0, _position);
		envRandom->reset;
		int discard = POS_MINOR1(_position.integerY) * MINOR_SIZE + POS_MINOR1(_position.integerX);
		if (discard < 0)
			discard = (MINOR_SIZE * MINOR_SIZE) / 2 + abs(discard);
		envRandom->discardNext(discard);
		return std::shared_ptr<Sector>(new Sector(parent, POS_MINOR1(_position.integerX), POS_MINOR1(_position.integerY), envRandom->uirandom(), 5, this));
		break;

	default:
		spehs::exceptions::fatalError("Invalid SECTORTYPE in getSectorFrom (Environment)!");
		break;
	}
}
std::shared_ptr<Sector> Environment::getSectorFrom(const SECTORTYPE _type, const spehs::WorldPosition& _position)
{
	switch (_type)
	{
	case MAJOR0:
		for (unsigned i = 0; i < data.major0.size(); i++)
		{
			if (data.major0[i]->checkPosition(POS_MAJOR0(_position.integerX), POS_MAJOR0(_position.integerY)))
			{
				return data.major0[i];
			}
		}
		break;

	case MAJOR1:
		for (unsigned i = 0; i < data.major1.size(); i++)
		{
			if (data.major1[i]->checkPosition(POS_MAJOR1(_position.integerX), POS_MAJOR1(_position.integerX)))
			{
				return data.major1[i];
			}
		}
		break;

	case MAJOR2:
		for (unsigned i = 0; i < data.major2.size(); i++)
		{
			if (data.major2[i]->checkPosition(POS_MAJOR2(_position.integerX), POS_MAJOR2(_position.integerX)))
			{
				return data.major2[i];
			}
		}
		break;

	case MAJOR3:
		for (unsigned i = 0; i < data.major3.size(); i++)
		{
			if (data.major3[i]->checkPosition(POS_MAJOR3(_position.integerX), POS_MAJOR3(_position.integerX)))
			{
				return data.major3[i];
			}
		}
		break;

	case MINOR0:
		for (unsigned i = 0; i < data.minor0.size(); i++)
		{
			if (data.minor0[i]->checkPosition(POS_MINOR0(_position.integerX), POS_MINOR0(_position.integerX)))
			{
				return data.minor0[i];
			}
		}
		break;

	case MINOR1:
		for (unsigned i = 0; i < data.minor1.size(); i++)
		{
			if (data.minor1[i]->checkPosition(POS_MINOR1(_position.integerX), POS_MINOR1(_position.integerX)))
			{
				return data.minor1[i];
			}
		}
		break;

	default:
		spehs::exceptions::fatalError("Invalid SECTORTYPE in getSectorFrom (Environment)!");
		break;
	}
	return generateSector(_type, _position);
}
std::shared_ptr<Sector> Environment::getAdjacent(const std::shared_ptr<Sector>& _sector, const DIRECTION _direction)
{
	switch (_direction)
	{
	case Environment::DOWN:
		getSectorFrom((SECTORTYPE) _sector->layer, getWorldPosition(_sector, 0, -1));
		break;

	case Environment::UP:
		getSectorFrom((SECTORTYPE) _sector->layer, getWorldPosition(_sector, 0, 1));
		break;

	case Environment::RIGHT:
		getSectorFrom((SECTORTYPE) _sector->layer, getWorldPosition(_sector, 1, 0));
		break;

	case Environment::LEFT:
		getSectorFrom((SECTORTYPE) _sector->layer, getWorldPosition(_sector, -1, 0));
		break;

	default:
		spehs::exceptions::fatalError("Invalid DIRECTION in getAdjacent (Environment)!");
		break;
	}
}
spehs::WorldPosition Environment::getWorldPosition(const std::shared_ptr<Sector>& _sector, const int16_t _xInc, const int16_t _yInc)
{
	switch (_sector->layer)
	{
	case MAJOR0:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MAJOR0_SIZE, (_sector->yInP + _yInc) * MAJOR0_SIZE);
		break;

	case MAJOR1:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MAJOR1_SIZE, (_sector->yInP + _yInc) * MAJOR1_SIZE) + getWorldPosition(_sector->parent);
		break;

	case MAJOR2:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MAJOR2_SIZE, (_sector->yInP + _yInc) * MAJOR2_SIZE) + getWorldPosition(_sector->parent);
		break;

	case MAJOR3:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MAJOR3_SIZE, (_sector->yInP + _yInc) * MAJOR3_SIZE) + getWorldPosition(_sector->parent);
		break;

	case MINOR0:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MINOR0_SIZE, (_sector->yInP + _yInc) * MINOR0_SIZE) + getWorldPosition(_sector->parent);
		break;

	case MINOR1:
		return spehs::WorldPosition((_sector->xInP + _xInc) * MINOR1_SIZE, (_sector->yInP + _yInc) * MINOR1_SIZE) + getWorldPosition(_sector->parent);
		break;

	default:
		spehs::exceptions::fatalError("Invalid SECTORTYPE in getWorldPosition (Environment)!");
		break;
	}
}