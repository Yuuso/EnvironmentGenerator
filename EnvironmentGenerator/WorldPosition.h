#pragma once
#include <glm/vec2.hpp>
#include <stdint.h>
#include <string>
#define CHUNK_WIDTH 256.0f

struct LayerPosition
{
	LayerPosition(){}
	LayerPosition(unsigned char _x, unsigned char _y) : x(_x), y(_y) {}
	unsigned char x;
	unsigned char y;
};
class ChunkPosition
{
public:
	ChunkPosition() : integerX(0), integerY(0){}
	ChunkPosition(const ChunkPosition& other) : integerX(other.integerX), integerY(other.integerY) {}
	ChunkPosition(uint64_t x, uint64_t y){ integerX = x; integerY = y; }
	bool operator==(const ChunkPosition& other) const { return integerX == other.integerX && integerY == other.integerY; }
	bool operator!=(const ChunkPosition& other) const { return integerX != other.integerX || integerY != other.integerY; }
	bool isNeighbour(const ChunkPosition& other) const { return integerX >= other.integerX - 1 && integerX <= other.integerX + 1 && integerY >= other.integerY - 1 && integerY <= other.integerY + 1; }

	glm::vec2 vec2To(const ChunkPosition& other) const;
	void getLayerPosition(LayerPosition* destination, int layerDepth) const { memcpy(&destination->x, (unsigned char*)&integerX + (7 - layerDepth), sizeof(unsigned char)); memcpy(&destination->y, (unsigned char*)&integerY + (7 - layerDepth), sizeof(unsigned char)); }

	size_t packetSize() const { return sizeof(uint64_t) * 2; }
	size_t read(unsigned char* buffer);
	size_t write(unsigned char* buffer) const;

	enum class Direction : int8_t { center = -1, northWest = 0, north = 1, northEast = 2, west = 3, east = 4, southWest = 5, south = 6, southEast = 7 };
	ChunkPosition getDirection(Direction direction)
	{
		switch (direction)
		{
		case Direction::center:			return ChunkPosition(integerX, integerY);
		case Direction::northWest:		return ChunkPosition(integerX - 1, integerY + 1);
		case Direction::north:			return ChunkPosition(integerX, integerY + 1);
		case Direction::northEast:		return ChunkPosition(integerX + 1, integerY + 1);
		case Direction::west:			return ChunkPosition(integerX - 1, integerY);
		case Direction::east:			return ChunkPosition(integerX + 1, integerY);
		case Direction::southWest:		return ChunkPosition(integerX - 1, integerY - 1);
		case Direction::south:			return ChunkPosition(integerX, integerY - 1);
		case Direction::southEast:		return ChunkPosition(integerX + 1, integerY - 1);
		}
	}
	inline ChunkPosition northWest() const { return ChunkPosition(integerX - 1, integerY + 1); }
	inline ChunkPosition north() const { return ChunkPosition(integerX, integerY + 1); }
	inline ChunkPosition northEast() const { return ChunkPosition(integerX + 1, integerY + 1); }
	inline ChunkPosition west() const { return ChunkPosition(integerX - 1, integerY); }
	inline ChunkPosition east() const { return ChunkPosition(integerX + 1, integerY); }
	inline ChunkPosition southWest() const { return ChunkPosition(integerX - 1, integerY - 1); }
	inline ChunkPosition south() const { return ChunkPosition(integerX, integerY - 1); }
	inline ChunkPosition southEast() const { return ChunkPosition(integerX + 1, integerY - 1); }

	uint64_t integerX;
	uint64_t integerY;

	std::string toString() const { return std::string("[" + std::to_string(integerX) + ", " + std::to_string(integerY) + "]"); }
private:
};

class WorldPosition : public ChunkPosition
{
public:
	WorldPosition() : ChunkPosition(), floatingX(0.0f), floatingY(0.0f) {}
	WorldPosition(const WorldPosition& other) : ChunkPosition(other), floatingX(other.floatingX), floatingY(other.floatingY) {}
	WorldPosition(uint64_t _integerX, uint64_t _integerY) : ChunkPosition(_integerX, _integerY), floatingX(0.0f), floatingY(0.0f){}
	WorldPosition(uint64_t _integerX, uint64_t _integerY, float _floatingX, float _floatingY) : ChunkPosition(_integerX, _integerY), floatingX(_floatingX), floatingY(_floatingY){}

	void operator=(const ChunkPosition& other){ integerX = other.integerX; integerY = other.integerY; }
	void operator=(const WorldPosition& other){ integerX = other.integerX; integerY = other.integerY; floatingX = other.floatingX; floatingY = other.floatingY; }
	bool operator==(const ChunkPosition& other){return other.integerX == integerX && other.integerY == integerY;}
	bool operator!=(const ChunkPosition& other){return other.integerX != integerX || other.integerY != integerY;}
	void operator+=(const glm::vec2& vec){ floatingX += vec.x; floatingY += vec.y; }
	void operator-=(const glm::vec2& vec){ floatingX -= vec.x; floatingY -= vec.y; }
	WorldPosition operator+(const glm::vec2& vec);
	WorldPosition operator-(const glm::vec2& vec);
	glm::vec2 vec2To(const ChunkPosition& other) const;
	glm::vec2 vec2To(const WorldPosition& other) const;

	size_t packetSize() const { return sizeof(uint64_t) * 2 + sizeof(float) * 2; }
	size_t read(unsigned char* buffer);
	size_t write(unsigned char* buffer) const;

	void update();///< Update checks whether floating coordinates overlap chunk border and modify chunk position accordingly

	float floatingX;
	float floatingY;
private:
};