
#pragma once

#include <stdint.h>
#include <vector>

#define MAJOR_SIZE 4096
#define MINOR_SIZE 256


typedef uint8_t SectorDataType;

namespace spehs{ class Polygon; }

struct SECTORDATA
{
	SectorDataType density;
	SectorDataType temperature;
	SectorDataType technology;
};

enum SECTORTYPE : bool
{
	MAJOR = true,
	MINOR = false
};

class Sector
{
public:
	Sector(const SECTORDATA _parentData, const SECTORTYPE _sectorbuild, const uint8_t _layer);
	~Sector();

	SECTORDATA getData(const uint16_t _x, const uint16_t _y);

private:
	void buildData(const SECTORDATA _parentData, const uint16_t _size);

	uint16_t factor;
	SECTORTYPE type;
	SECTORDATA** rawdata;

	std::vector<spehs::Polygon*> primaryVisuals;
	std::vector<spehs::Polygon*> secondaryVisuals;
};

