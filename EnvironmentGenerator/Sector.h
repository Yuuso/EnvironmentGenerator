
#pragma once

#include <stdint.h>
#include <vector>


typedef uint8_t SectorDataType;

namespace spehs{ class Polygon; class WorldPosition; }

struct SECTORDATA
{
	SectorDataType density;
	SectorDataType temperature;
	SectorDataType technology;
};

class Sector
{
public:
	Sector();
	Sector(const spehs::WorldPosition& _position);
	~Sector();
	
private:
	uint32_t localSeed;
	SECTORDATA rawdata;
	
	std::vector<spehs::Polygon*> primaryVisuals;
	std::vector<spehs::Polygon*> secondaryVisuals;
};

