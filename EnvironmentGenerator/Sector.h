
#pragma once

#include <stdint.h>
#include <vector>

#define MAJOR_SIZE 4096
#define MINOR_SIZE 256


typedef uint8_t SectorDataType;

namespace spehs{ class Polygon; }

struct SECTORDATA
{
	SectorDataType density; //Void areas in Spehs
	SectorDataType temperature;
	SectorDataType technology;
	//Something for asteroid building?
};

class Sector
{
public:
	Sector();
	Sector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer);
	~Sector();

	void buildSector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer);
	SECTORDATA* getData();

private:
	void buildData(const SECTORDATA _parentData, const uint16_t _size);
	void clearData();
	void clearVisuals();

	uint32_t localSeed;
	uint16_t factor;
	SECTORTYPE type;

	SECTORDATA rawdata;
	SECTORDATA* data;

	Sector* parent;
	uint16_t xInP, yInP;
};

