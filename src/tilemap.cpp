#include "tilemap.h"
#include "game.h"
#include "quadbatch.h"
#include "error.h"


TileMap::TileMap(float yMax, const std::vector<uint16_t>& data) : Model<primitives::Quad>() 
    , _yMax(yMax)
{
    
    _batchId = data[0];
    _textureId = data[1];
    _tileSize = data[2];
    auto* batch = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(_batchId));

    M_Assert(batch->getTextureWidth() == batch->getTextureHeight(), "Require square texture maps");
    _tw = _tileSize / batch->getTextureWidth();
    _tilesPerRow = batch->getTextureWidth() / _tileSize;
    M_Assert(_tilesPerRow > 0 && (_tilesPerRow & (_tilesPerRow - 1)) == 0, " [TileMap]: size must be a power of 2");
    _n = std::log2(_tilesPerRow);


    size_t pos{ 3 };
    int x{ 0 };
    int y{ 0 };
    _z = 0.f;
    _width = 0;
    std::list<std::pair<int, size_t>> loop;

    while (pos < data.size()) {
        auto word = data[pos++];
        //if (chunk == ChunkType::EndOfMap) break;

        // this is how it works. We hav just a sequence of tiles
        // each tile specified by a 16 bit number
		// if msb is 1, then this is a special chunk
        // for instance, 0x8001 --> RLE 
        // in this case the 1st number is the count, 2nd is the tile
        // 0x8002 --> specifies the width of the map. When x reaches the end
		// for the line, x is reset to 0 and y is incremented
        if ((word & 0x8000) != 0) {
            ChunkType chunk = static_cast<ChunkType>(word);
            switch (chunk) {
            case ChunkType::RLE: {
                uint16_t count = data[pos++];
                uint16_t tile = data[pos++];

                for (int i = 0; i < count; ++i) {
                    addTile(x, y, tile);
                    x++;
                    if (_width > 0 && x >= _width) {
                        x = 0;
                        y++;
                    }
                }
                break;
            }

            case ChunkType::SetWidth: {
                _width = data[pos++];
                break;
            }
            case ChunkType::SetYBase: {
				auto yBase = data[pos++];
                _z = 1.f - (_tileSize*yBase) / _yMax;
                break;
            }
            case ChunkType::SetBackground: {
                _z = 0.f;
                break;

            }
            case ChunkType::SetBackgroundLayer: {
                _z = -data[pos++];
                break;

            }
            case ChunkType::Loop: {
                int count = data[pos++];
                loop.push_back({ count, pos });
                break;
            }
            case ChunkType::LoopEnd: {
                if (loop.empty()) {
                    GLIB_FAIL("[TileMap] Loop end outside of loop.");
                }
                loop.front().first--;
                if (loop.front().first == 0) {
                    loop.pop_front();
                }
                else {
                    pos = loop.front().second;
                }
                break;
            }
            case ChunkType::Skip: {
                x++;
                if (_width > 0 && x >= _width) {
                    x = 0;
                    y++;
                }
                break;
            }
            case ChunkType::GoTo: {
                x = data[pos++];
                y = data[pos++];
                break;
            }
            case ChunkType::Back: {
                if (_width > 0 && x == 0) {
                    x = _width - 1;
                    y--;
                }
                else {
                    x--;
                }
                break;
            }
            }
        } else {
			// this is a tile
			// if x >= width, then we need to reset x and increment y
			addTile(x, y, word);
			x++;
			if (_width > 0 && x >= _width) {
				x = 0;
				y++;
			}
		}



   
    }

}


void TileMap::emitTiles(int tile, int count, int x, int y) {
    for (int i = 0; i < count; ++i) {
        addTile(x + i, y, tile);
    }
}

void TileMap::fillRegion(int tile, int x, int y, int w, int h) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            addTile(x + i, y + j, tile);
        }
    }
}




void TileMap::addTile(int x, int y, int tile) {
    // First of all find texture coordinates
    if (tile == 0x8007) return;
    uint16_t tileNumber = tile & 0x0FFF; // mask lower 12 bits
    uint8_t effects = tile >> 12; // shift right by 12 bits

	bool flipX = (effects & 0x01) != 0;

    int row = tileNumber >> _n;
    int col = tileNumber & ((1 << _n) - 1);
    glm::vec4 texCoordinates(col * _tw, row * _tw, _tw, _tw);
    if (flipX) {
        texCoordinates.x += _tw;
        texCoordinates[2] = -_tw;
    }
	_prims.push_back(primitives::Quad(
		texCoordinates,
		glm::vec2(-x * _tileSize, -y * _tileSize),
		glm::vec2(_tileSize, _tileSize),
        _textureId,
        _z));
    

}


//void TileMap::addFrame(const std::vector<int> &data, int ticks) {
//    size_t i{0};
//    // for each frame we store offset and length
//    int ix{0};
//    int iy{0};
//    auto offset = _prims.size();
//    while (i < data.size()) {
//        if (data[i] >= 0) {
//            // this is a square tile
//            int row = data[i] / _tilesPerCol;
//            int col = data[i] % _tilesPerRow;
//            glm::vec2 topLeft (col * _tw, row * _th);
//            _prims.push_back(primitives::Quad(
//                    glm::vec4(topLeft.x, topLeft.y, _tw, _th),
//                    glm::vec2(-ix * _tileSize.x, -iy * _tileSize.y),
//                    _tileSize,
//                    0));
//            ix++;
//            if (ix >= _width) {
//                ix = 0;
//                iy++;
//            }
//            i++;
//        }
//    }
//    auto frameQuads = _prims.size() - offset;
//    _frameInfo.push_back(FrameInfo{offset, frameQuads, ticks});
//    _maxFrameLength = std::max(_maxFrameLength, frameQuads);
//}
//
std::shared_ptr<IRenderer> TileMap::getRenderer(int) {
    return std::make_shared<Renderer<Model<primitives::Quad>>>(this, _batchId);

}


//void TileMapRenderer::updateGeometry() {
//    const auto& frameInfo = _model->getFrameInfo(_frame);
//
//    auto worldTransform = _node->getWorldMatrix();
//    for (auto i = 0; i < frameInfo.length; ++i) {
//        _model->get(frameInfo.offset + i).transform(_vertices[i], worldTransform, _multiplyColor);
//    }
//}
//void TileMapRenderer::update() {
//    if (!_started) {
//        return;
//    }
//    const auto& frameInfo = _model->getFrameInfo(_frame);
//    _tickCount++;
//    if (_tickCount > frameInfo.ticks) {
//        // update frame
//        auto oldFrame = _frame;
//        _frame++;
//        _tickCount = 0;
//        if (_frame >= _model->getFrameCount()) {
//            _frame = 0;
//        }
//        if (oldFrame != _frame) {
//            _dirty = true;
//        }
//    }
//    if (_dirty) {
//        updateGeometry();
//        _dirty = false;
//    }


//}
