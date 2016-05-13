
#pragma once

#include "Environment.h"

#include <stdint.h>
#include <vector>
#include <memory>


namespace spehs{ class Polygon; }


class Sector
{
	friend class Environment;
public:
	Sector(const std::shared_ptr<Sector>& _parent, const uint16_t _x, const uint16_t _y, const uint32_t _localSeed, const uint8_t _layer, Environment* _creator);
	~Sector();

	std::shared_ptr<SECTORDATA> getData();
	bool checkPosition(const uint16_t _x, const uint16_t _y);

protected:
	uint16_t layer;
	uint16_t xInP;
	uint16_t yInP;
	uint32_t localSeed;

	SECTORDATA rawdata;
	std::shared_ptr<SECTORDATA> data;
	std::shared_ptr<Sector> parent;
	Environment* creator;

	//DEBUG VISUAL
	spehs::Polygon* visual;
};

