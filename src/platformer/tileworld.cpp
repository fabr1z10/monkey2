#include "tileworld.h"
#include "../model.h"
#include "../error.h"

TileWorld::TileWorld(int size, int batchId, const std::vector<uint8_t>& data) : Node(), _batchId(batchId) {
	M_Assert(size > 0 && (size & (size - 1)) == 0, " [Tileworld]: size must be a power of 2");
	_n = std::log2(size);
	_size = (float)size;

    _tileFactory = {
        {0, [&]() { return std::make_unique<SolidTile>(_size); }},
        {1, [&]() { return std::make_unique<SemiSolidTile>(_size); }}

    };
   
   // Add more tile types here
   



    size_t pos{ 0 };
    int x{ 0 };
    int y{ 0 };
        
    while (pos < data.size()) {
        ChunkType chunk = static_cast<ChunkType>(data[pos++]);
        if (chunk == ChunkType::EndOfMap) break;

        switch (chunk) {
        case ChunkType::RLE: {
            uint8_t tile = data[pos++];
            uint8_t count = data[pos++];
            emitTiles(tile, count,x, y);
            x += count;
            break;
        }
        case ChunkType::Region: {
            uint8_t tile = data[pos++];
            uint8_t x = data[pos++];
            uint8_t y = data[pos++];
            uint8_t w = data[pos++];
            uint8_t h = data[pos++];
            fillRegion(tile, x, y, w, h);
			x += w;
            break;
        }
        case ChunkType::RawLine: {
            uint8_t count = data[pos++];
            for (int i = 0; i < count; ++i) {
                uint8_t tile = data[pos++];
                emitTile(tile, x++, y);                
            }
            break;
        }
		case ChunkType::MoveCursor: {
			x = data[pos++];
			y = data[pos++];
			break;
		}
        default:
            std::cerr << "Unknown chunk type: " << static_cast<int>(chunk) << "\n";
            return;
        }
    }

	
}	

void TileWorld::emitTile(int tile, int x, int y) {
    addTile(x, y, _tileFactory[tile]());
}

void TileWorld::emitTiles(int tile, int count, int x, int y) {
	for (int i = 0; i < count; ++i) {
		addTile(x + i, y, _tileFactory[tile]());
	}
}

void TileWorld::fillRegion(int tile, int x, int y, int w, int h) {
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			addTile(x + i, y + j, _tileFactory[tile]());
		}
	}
}




void TileWorld::addTile(int x, int y, std::unique_ptr<Tile> tile) {
	_tiles[{x, y}] = std::move(tile);
}
void TileWorld::start() {
    Node::start();
	std::vector<float> debugModelData;
	for (const auto& [a, b] : _tiles) {
		float x = a.first << _n;
		float y = a.second << _n;
		auto data = b->getRaw(x, y);
		
		debugModelData.insert(debugModelData.end(), data.begin(), data.end());	
	}


	auto model = std::make_shared<Model<primitives::Line>>(debugModelData);
	setModel(model, _batchId);

}

TileCollisionResult TileWorld::hor(float x0, float y0, float x1) {
    int ix0 = int(x0) >> _n;
    int iy = int(y0) >> _n;
    int ix1 = int(x1) >> _n;
    int inc = (ix1 > ix0) ? 1 : -1;
    int ix = ix0;
	for (int i = 0;i <= abs(ix0 - ix1); i++) {
        auto it = _tiles.find({ ix, iy });
        if (it != _tiles.end()) {
            auto result = it->second->ver(x0, y0, x1);
            if (result.collide) {
                return result;
            }
        }
        ix += inc;
    }
    return TileCollisionResult();
}

TileCollisionResult TileWorld::ver(float x0, float y0, float y1) {
	int ix = int(x0) >> _n;
	int iy0 = int(y0) >> _n;
	int iy1 = int(y1) >> _n;
	int inc = (iy1 > iy0) ? 1 : -1;
    int iy = iy0;
    for (int i = 0; i <= abs(iy0 - iy1); i++) {
		auto it = _tiles.find({ ix, iy });
		if (it != _tiles.end()) {
			auto result = it->second->ver(x0, y0, y1);
            if (result.collide) {
                return result;
            }
		}
		iy += inc;
	}
    return TileCollisionResult();
}