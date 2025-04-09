#pragma once

#include "../node.h"
#include "tile.h"
#include "../hashpair.h"
#include <functional>

enum class ChunkType : uint8_t {
	RLE = 0x01,
	Region = 0x02,
	RawLine = 0x03,
	MoveCursor = 0x04,
	EndOfMap = 0xFF
};

class TileWorld : public Node {

public:

	TileWorld(int size, int batchId, const std::vector<uint8_t>& data);

	void addTile(int x, int y, std::unique_ptr<Tile>);

	void start() override;

	TileCollisionResult ver(float x0, float y0, float y1);

	TileCollisionResult hor(float x0, float y0, float x1);

private:
	float _size;
	int _n;
	int _batchId;
	std::unordered_map<std::pair<int, int>, std::unique_ptr<Tile> > _tiles;
	void emitTile(int tile, int x, int y);
	void emitTiles(int tile, int count, int x, int y);
	void fillRegion(int tile, int x, int y, int w, int h);
	std::unordered_map<int, std::function<std::unique_ptr<Tile>()>> _tileFactory;
};

