
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
	Sector();
	Sector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer);
	~Sector();

	void buildSector(Sector* _parent, const uint16_t _x, const uint16_t _y, const SECTORTYPE _sectorbuild, const uint8_t _layer);
	SECTORDATA getData(const uint16_t _x, const uint16_t _y);
	void renderVisuals(const bool _state, const unsigned char _alpha = 255);

private:
	void buildData(const SECTORDATA _parentData, const uint16_t _size);
	void clearData();
	void clearVisuals();

	uint32_t localSeed;
	uint16_t factor;
	SECTORTYPE type;
	SECTORDATA** rawdata;

	Sector* parent;
	uint16_t xInP, yInP;

	std::vector<spehs::Polygon*>* primaryVisuals;
	std::vector<spehs::Polygon*>* secondaryVisuals;
};

