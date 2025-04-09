#include "tile.h"

Tile::Tile(float size) : _size(size) {}

TileCollisionResult SolidTile::hor(float x0, float y0, float x1) {
	TileCollisionResult result;
	result.collide = true;
	result.position = x0;
	result.normal = glm::vec2(x1 >= x0 ? -1.f : 1.f, 0.f);
	return result;
}

TileCollisionResult SolidTile::ver(float x0, float y0, float y1) {
	TileCollisionResult result;
	result.collide = true;
	result.position = y0;
	result.normal = glm::vec2(y1 >= y0 ? -1.f : 1.f, 0.f);
	return result;
}

std::vector<float> SolidTile::getRaw(float x, float y) {
	return {
		x, y, 0.f, x + _size, y, 0.f,
		x + _size, y, 0.f, x + _size, y + _size, 0.f,
		x + _size, y + _size, 0.f, x, y + _size, 0.f,
		x, y + _size, 0.f, x, y, 0.f };
}


TileCollisionResult SemiSolidTile::hor(float x0, float y0, float x1) {
	return TileCollisionResult();
}

TileCollisionResult SemiSolidTile::ver(float x0, float y0, float y1) {
	if (y0 > _height && y1 < _height) {
		TileCollisionResult result;
		result.collide = true;
		result.position = _height;
		result.normal = glm::vec2(0.f, 1.f);
		return result;
	} 
	return TileCollisionResult();
}

std::vector<float> SemiSolidTile::getRaw(float x, float y) {
	return {
		x, y, 0.f, x + _size, y, 0.f,
		x + _size, y, 0.f, x + _size, y + _size, 0.f,
		x + _size, y + _size, 0.f, x, y + _size, 0.f,
		x, y+_size, 0.f, x, y, 0.f };
}