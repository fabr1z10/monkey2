#pragma once

#include "glm/glm.hpp"
#include <vector>

class Tile;

struct TileCollisionResult {
	TileCollisionResult() : collide(false), position(0.f), tile(nullptr) {}

	bool collide;
	float position;
	glm::vec2 normal;
	Tile* tile;
};

class Tile {

public:
	Tile(float size);

	virtual TileCollisionResult hor(float x0, float y0, float x1) = 0;

	virtual TileCollisionResult ver(float x0, float y0, float y1) = 0;
	
	virtual std::vector<float> getRaw(float x, float y) = 0;
protected:
	float _size;
};



class SolidTile : public Tile {
public:
	using Tile::Tile;

	TileCollisionResult hor(float x0, float y0, float x1) override;

	TileCollisionResult ver(float x0, float y0, float y1) override;

	std::vector<float> getRaw(float x, float y) override;
};

class SemiSolidTile : public Tile {
public:
	using Tile::Tile;

	TileCollisionResult hor(float x0, float y0, float x1) override;

	TileCollisionResult ver(float x0, float y0, float y1) override;

	std::vector<float> getRaw(float x, float y) override;
private:
	float _height;
};
