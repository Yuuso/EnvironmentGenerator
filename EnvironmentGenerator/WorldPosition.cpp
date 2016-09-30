#include <memory>
#include <iostream>
#include "WorldPosition.h"

size_t ChunkPosition::read(unsigned char* buffer)
{
	size_t offset(0);
	memcpy(&integerX, &buffer[offset], sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&integerY, &buffer[offset], sizeof(uint64_t));		offset += sizeof(uint64_t);
	return offset;
}
size_t ChunkPosition::write(unsigned char* buffer) const
{
	size_t offset(0);
	memcpy(&buffer[offset], &integerX, sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&buffer[offset], &integerY, sizeof(uint64_t));		offset += sizeof(uint64_t);
	return offset;
}
size_t WorldPosition::read(unsigned char* buffer)
{
	size_t offset(0);
	memcpy(&integerX, &buffer[offset], sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&integerY, &buffer[offset], sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&floatingX, &buffer[offset], sizeof(float));			offset += sizeof(float);
	memcpy(&floatingY, &buffer[offset], sizeof(float));			offset += sizeof(float);
	return offset;
}
size_t WorldPosition::write(unsigned char* buffer) const
{
	size_t offset(0);
	memcpy(&buffer[offset], &integerX, sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&buffer[offset], &integerY, sizeof(uint64_t));		offset += sizeof(uint64_t);
	memcpy(&buffer[offset], &floatingX, sizeof(float));			offset += sizeof(float);
	memcpy(&buffer[offset], &floatingY, sizeof(float));			offset += sizeof(float);
	return offset;
}

glm::vec2 ChunkPosition::vec2To(const ChunkPosition& other) const
{
	return glm::vec2(
		CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerX - integerX),
		CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerY - integerY));
}
WorldPosition WorldPosition::operator+(const glm::vec2& vec)
{
	WorldPosition position(*this);
	position += vec;
	position.update();
	return position;
}
WorldPosition WorldPosition::operator-(const glm::vec2& vec)
{
	WorldPosition position(*this);
	position -= vec;
	position.update();
	return position;
}
glm::vec2 WorldPosition::vec2To(const ChunkPosition& other) const
{
	return glm::vec2(
		CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerX - integerX),
		CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerY - integerY));
}
glm::vec2 WorldPosition::vec2To(const WorldPosition& other) const
{
	return glm::vec2(
		other.floatingX - floatingX + CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerX - integerX),
		other.floatingY - floatingY + CHUNK_WIDTH * int64_t/*cast to signed!*/(other.integerY - integerY));
}
void WorldPosition::update()
{
	if (floatingX < 0.0f || floatingX >= CHUNK_WIDTH)
	{
		int64_t difference(std::floor(floatingX / CHUNK_WIDTH));
		floatingX -= CHUNK_WIDTH * difference;
		integerX += difference;
	}
	if (floatingY < 0.0f || floatingY >= CHUNK_WIDTH)
	{
		int64_t difference(std::floor(floatingY / CHUNK_WIDTH));
		floatingY -= CHUNK_WIDTH * difference;
		integerY += difference;
	}
}