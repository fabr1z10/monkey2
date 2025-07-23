//#include "tilemap.h"
//#include "game.h"
//#include "quadbatch.h"
//#include "error.h"
//#include "model.h"
//#include "util.h"
//
//TileMap::TileMap(int tileSize, int batchId, int texId) : Node(), camX(0.f), camY(0.f), _batchId(batchId),
//_textureId(texId)
//{
//    M_Assert(tileSize > 0 && (tileSize & (tileSize - 1)) == 0, " [TileMap]: tile size must be a power of 2");
//    _n = std::log2(tileSize);
//    _tileSize = tileSize;
//
//    auto* batch = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId));
//    auto atlasSize = batch->getTextureSize();
//    auto cameraId = batch->getCameraId();
//
//    Camera* cam = Game::instance().getRoom()->getCamera(cameraId);
//    auto viewport = cam->getViewport();
//    // calculate width and height of viewport in tiles
//    _screenTilesPerRow = viewport[2] / _tileSize;
//    _screenTilesPerCol = viewport[3] / _tileSize;
//    _atlasTilesPerRow = atlasSize.x >> _n;
//    _atlasTilesPerCol = atlasSize.y >> _n;
//	_shiftAtlas = (int) std::log2(_atlasTilesPerRow);
//	_tileNormalizedLength = glm::vec2(1.f / _atlasTilesPerRow, 1.f / _atlasTilesPerCol);
//
//    int gridWidth = _screenTilesPerRow + 2;
//    int gridHeight = _screenTilesPerCol + 2;
//    int quadCount = gridWidth * gridHeight;
//
//    // create the model, with the correct number of quads
//    auto model = std::make_shared<Model<primitives::Quad>>(quadCount);
//    _model = model;
//
//    std::cout << " atlas size: " << atlasSize << std::endl;
//
//    // create renderer!
//    _renderer = std::make_shared<TileMapRenderer>(this, model.get(), _batchId);
//
//	_opcodes["tle"] = &TileMap::tle;
//  //
//  //  _batchId = data[0];
//  //  _textureId = data[1];
//  //  _tileSize = data[2];
//
//  //  M_Assert(batch->getTextureWidth() == batch->getTextureHeight(), "Require square texture maps");
//  //  _tw = _tileSize / batch->getTextureWidth();
//  //  _tilesPerRow = batch->getTextureWidth() / _tileSize;
//  //  M_Assert(_tilesPerRow > 0 && (_tilesPerRow & (_tilesPerRow - 1)) == 0, " [TileMap]: size must be a power of 2");
//  //  _n = std::log2(_tilesPerRow);
//
//
//  //  size_t pos{ 3 };
//  //  int x{ 0 };
//  //  int y{ 0 };
//  //  _z = 0.f;
//  //  _width = 0;
//  //  std::list<std::pair<int, size_t>> loop;
//
//  //  while (pos < data.size()) {
//  //      auto word = data[pos++];
//  //      //if (chunk == ChunkType::EndOfMap) break;
//
//  //      // this is how it works. We hav just a sequence of tiles
//  //      // each tile specified by a 16 bit number
//		//// if msb is 1, then this is a special chunk
//  //      // for instance, 0x8001 --> RLE
//  //      // in this case the 1st number is the count, 2nd is the tile
//  //      // 0x8002 --> specifies the width of the map. When x reaches the end
//		//// for the line, x is reset to 0 and y is incremented
//  //      if ((word & 0x8000) != 0) {
//  //          ChunkType chunk = static_cast<ChunkType>(word);
//  //          switch (chunk) {
//  //          case ChunkType::RLE: {
//  //              uint16_t count = data[pos++];
//  //              uint16_t tile = data[pos++];
//
//  //              for (int i = 0; i < count; ++i) {
//  //                  addTile(x, y, tile);
//  //                  x++;
//  //                  if (_width > 0 && x >= _width) {
//  //                      x = 0;
//  //                      y++;
//  //                  }
//  //              }
//  //              break;
//  //          }
//
//  //          case ChunkType::SetWidth: {
//  //              _width = data[pos++];
//  //              break;
//  //          }
//  //          case ChunkType::SetYBase: {
//		//		auto yBase = data[pos++];
//  //              _z = 1.f - (_tileSize*yBase) / _yMax;
//  //              break;
//  //          }
//  //          case ChunkType::SetBackground: {
//  //              _z = 0.f;
//  //              break;
//
//  //          }
//  //          case ChunkType::SetBackgroundLayer: {
//  //              _z = -data[pos++];
//  //              break;
//
//  //          }
//  //          case ChunkType::Loop: {
//  //              int count = data[pos++];
//  //              loop.push_back({ count, pos });
//  //              break;
//  //          }
//  //          case ChunkType::LoopEnd: {
//  //              if (loop.empty()) {
//  //                  GLIB_FAIL("[TileMap] Loop end outside of loop.");
//  //              }
//  //              loop.front().first--;
//  //              if (loop.front().first == 0) {
//  //                  loop.pop_front();
//  //              }
//  //              else {
//  //                  pos = loop.front().second;
//  //              }
//  //              break;
//  //          }
//  //          case ChunkType::Skip: {
//  //              x++;
//  //              if (_width > 0 && x >= _width) {
//  //                  x = 0;
//  //                  y++;
//  //              }
//  //              break;
//  //          }
//  //          case ChunkType::GoTo: {
//  //              x = data[pos++];
//  //              y = data[pos++];
//  //              break;
//  //          }
//  //          case ChunkType::Back: {
//  //              if (_width > 0 && x == 0) {
//  //                  x = _width - 1;
//  //                  y--;
//  //              }
//  //              else {
//  //                  x--;
//  //              }
//  //              break;
//  //          }
//  //          }
//  //      } else {
//		//	// this is a tile
//		//	// if x >= width, then we need to reset x and increment y
//		//	addTile(x, y, word);
//		//	x++;
//		//	if (_width > 0 && x >= _width) {
//		//		x = 0;
//		//		y++;
//		//	}
//		//}
//
//
//
//  //
//  //  }
//
//}
//
//TileMapRenderer::TileMapRenderer(TileMap* map, Model<primitives::Quad>* model, int batch) :
//    Renderer<Model<primitives::Quad>>(model, batch), _map(map) {
//    setNode(map);
//}
//
//void TileMap::tle(const std::vector<std::string>& args) {
//	for (const auto& arg : args) {
//		std::cout << " adding tile: " << arg << " at " << _cx << ", " << _cy << "\n";
//		_tiles[{_cx, _cy}] = Tile{static_cast<uint16_t>(std::stoul(arg)), _fx};
//		_cx++;
//	}
//}
//std::vector<std::string> TileMap::splitByNewline(const std::string& input) {
//	std::vector<std::string> lines;
//	std::stringstream ss(input);
//	std::string line;
//	while (std::getline(ss, line)) {
//		lines.push_back(line);
//	}
//	return lines;
//}
//
//void TileMap::removeSpacesAndTabs(std::string& str) {
//	str.erase(std::remove_if(str.begin(), str.end(),
//							 [](unsigned char c) { return c == ' ' || c == '\t'; }),
//			  str.end());
//}
//
//std::vector<std::string> TileMap::splitArgs(const std::string& argStr) {
//	std::vector<std::string> result;
//	std::stringstream ss(argStr);
//	std::string token;
//	while (std::getline(ss, token, ',')) {
//		if (!token.empty())
//			result.push_back(token);
//	}
//	return result;
//}
//
//void TileMap::define(const std::string & data) {
//	// reset pointers @ bottom left
//	_cx = 0;
//	_cy = 0;
//	_fx = 0;
//	auto nn = splitByNewline(data);
//	for (auto& line : nn) {
//
//		size_t semicolonPos = line.find(';');
//		if (semicolonPos != std::string::npos) {
//			line = line.substr(0, semicolonPos);
//		}
//		removeSpacesAndTabs(line);
//		if (line.empty()) continue;
//		if (line.length() < 3) {
//			GLIB_FAIL("[TileMap] Syntax error in tile definition - line with less than 3 characters: " + line);
//		}
//
//		std::string opcode = line.substr(0, 3);
//
//		auto args = splitArgs(line.substr(3));
//
//		auto it = _opcodes.find(opcode);
//		if (it == _opcodes.end()) {
//			GLIB_FAIL("[TileMap] Unknown opcode: " + opcode);
//		}
//		(this->*it->second)(args);
//
//		//std::cout << line << "\n";
//	}
//}
//
//void TileMapRenderer::start() {
//	Renderer<Model<primitives::Quad>>::start();
//	int k=0;
//	auto* v = _vertices[k++];
//	auto ts = 1.f / 64.f;
//	auto tc = _map->getScreenTileCount();
//	auto tileSize = 8.f;
//	for (int i = 0; i< tc.y; ++i) {
//		float y = tileSize*i;
//		for (int j = 0; j < tc.x; j++) {
//			float x = tileSize * j;
//			v->position = glm::vec3(x, y, 0.f);
//			v->color = glm::vec4(1.f);
//			v->texIndex = _map->getTextureId();
//			v->texCoord = glm::vec2(0.f, 0.f);
//
//			(v+1)->position = glm::vec3(x+tileSize, y, 0.f);
//			(v+1)->color = glm::vec4(1.f);
//			(v+1)->texIndex =  _map->getTextureId();;
//			(v+1)->texCoord = glm::vec2(0.f, 0.f);
//
//			(v+2)->position = glm::vec3(x+tileSize, y+tileSize, 0.f);
//			(v+2)->color = glm::vec4(1.f);
//			(v+2)->texIndex =  _map->getTextureId();;
//			(v+2)->texCoord = glm::vec2(0.f, 0.f);
//
//			(v+3)->position = glm::vec3(x, y+tileSize, 0.f);
//			(v+3)->color = glm::vec4(1.f);
//			(v+3)->texIndex =  _map->getTextureId();;
//			(v+3)->texCoord = glm::vec2(0.f, 0.f);
//			v= _vertices[k++];
//		}
//
//	}
//
//}
//void TileMap::emitTiles(int tile, int count, int x, int y) {
//    for (int i = 0; i < count; ++i) {
//        addTile(x + i, y, tile);
//    }
//}
//
//void TileMap::fillRegion(int tile, int x, int y, int w, int h) {
//    for (int i = 0; i < w; ++i) {
//        for (int j = 0; j < h; ++j) {
//            addTile(x + i, y + j, tile);
//        }
//    }
//}
//
//
//
//
//void TileMap::addTile(int x, int y, int tile) {
// //   // First of all find texture coordinates
// //   if (tile == 0x8007) return;
// //   uint16_t tileNumber = tile & 0x0FFF; // mask lower 12 bits
// //   uint8_t effects = tile >> 12; // shift right by 12 bits
//
//	//bool flipX = (effects & 0x01) != 0;
//
// //   int row = tileNumber >> _n;
// //   int col = tileNumber & ((1 << _n) - 1);
// //   glm::vec4 texCoordinates(col * _tw, row * _tw, _tw, _tw);
// //   if (flipX) {
// //       texCoordinates.x += _tw;
// //       texCoordinates[2] = -_tw;
// //   }
//	//_prims.push_back(primitives::Quad(
//	//	texCoordinates,
//	//	glm::vec2(-x * _tileSize, -y * _tileSize),
//	//	glm::vec2(_tileSize, _tileSize),
// //       _textureId,
// //       _z));
// //
//
//}
//
//
////void TileMap::addFrame(const std::vector<int> &data, int ticks) {
////    size_t i{0};
////    // for each frame we store offset and length
////    int ix{0};
////    int iy{0};
////    auto offset = _prims.size();
////    while (i < data.size()) {
////        if (data[i] >= 0) {
////            // this is a square tile
////            int row = data[i] / _tilesPerCol;
////            int col = data[i] % _tilesPerRow;
////            glm::vec2 topLeft (col * _tw, row * _th);
////            _prims.push_back(primitives::Quad(
////                    glm::vec4(topLeft.x, topLeft.y, _tw, _th),
////                    glm::vec2(-ix * _tileSize.x, -iy * _tileSize.y),
////                    _tileSize,
////                    0));
////            ix++;
////            if (ix >= _width) {
////                ix = 0;
////                iy++;
////            }
////            i++;
////        }
////    }
////    auto frameQuads = _prims.size() - offset;
////    _frameInfo.push_back(FrameInfo{offset, frameQuads, ticks});
////    _maxFrameLength = std::max(_maxFrameLength, frameQuads);
////}
////
////std::shared_ptr<IRenderer> TileMap::getRenderer(int) {
////    return std::make_shared<Renderer<Model<primitives::Quad>>>(this, _batchId);
////
////}
//
//
//void TileMapRenderer::updateGeometry() {
////    const auto& frameInfo = _model->getFrameInfo(_frame);
////
////    auto worldTransform = _node->getWorldMatrix();
////    for (auto i = 0; i < frameInfo.length; ++i) {
////        _model->get(frameInfo.offset + i).transform(_vertices[i], worldTransform, _multiplyColor);
////    }
//}
//
//glm::vec4 TileMap::getTextureCoordinates(int tileId) {
//	int tileX = tileId & (_atlasTilesPerRow - 1);     // same as u % tilesPerRow
//	int tileY = tileId >> _shiftAtlas;                // same as u / tilesPerRow
//	float tx0 = tileX * _tileNormalizedLength.x;
//	float tx1 = tx0 + _tileNormalizedLength.x;
//	float ty0 = tileY * _tileNormalizedLength.y;
//	float ty1 = ty0 + _tileNormalizedLength.y;
//	return {tx0, tx1, ty0, ty1};
//}
//
//void TileMapRenderer::update() {
//    //std::cout << "qui\n";
//	// First, we get the camera position
//	glm::ivec2 bottomLeft;
//	glm::vec2 offset;
//	_map->getIndexOfBottomLeftTile(bottomLeft, offset);
//	auto tc = _map->getScreenTileCount();
//	int ex = offset.x == 0.f ? 0 : 1;
//	int ey = offset.y == 0.f ? 0 : 1;
//	int k = 0;
//	//auto atlasTilesPerRow = _map->getAtlasTilesPerRow();
//	//int shift = std::log2(atlasTilesPerRow);
//	for (int y = 0; y < tc.y + ey; y++) {
//		for (int x = 0; x < tc.x + ex; x++) {
//			int ctx = bottomLeft.x + x;
//			int cty = bottomLeft.y + y;
//			// get tile
//			auto* tile = _map->load(ctx, cty);
//			auto* v = _vertices[k++];
//			if (tile != nullptr) {
//				auto texCoords = _map->getTextureCoordinates(tile->tileId);
//				v->texCoord = glm::vec2(texCoords[0], texCoords[3]);
//				(v+1)->texCoord = glm::vec2(texCoords[1], texCoords[3]);
//				(v+2)->texCoord = glm::vec2(texCoords[1], texCoords[2]);
//				(v+3)->texCoord = glm::vec2(texCoords[0], texCoords[2]);
//			} else {
//				v->texCoord = glm::vec2(0.f);
//				(v+1)->texCoord = glm::vec2(0.f);
//				(v+2)->texCoord = glm::vec2(0.f);
//				(v+3)->texCoord = glm::vec2(0.f);
//
//			}
//		}
//	}
//
////    if (!_started) {
////        return;
////    }
////    const auto& frameInfo = _model->getFrameInfo(_frame);
////    _tickCount++;
////    if (_tickCount > frameInfo.ticks) {
////        // update frame
////        auto oldFrame = _frame;
////        _frame++;
////        _tickCount = 0;
////        if (_frame >= _model->getFrameCount()) {
////            _frame = 0;
////        }
////        if (oldFrame != _frame) {
////            _dirty = true;
////        }
////    }
////    if (_dirty) {
////        updateGeometry();
////        _dirty = false;
//}
//
//
////}
//
//void TileMap::getIndexOfBottomLeftTile(glm::ivec2& bottomLeft, glm::vec2& offset) const {
//	bottomLeft.x = (int) camX >> _n;
//	bottomLeft.y = (int) camY >> _n;
//	offset = glm::vec2(camX - (bottomLeft.x << _n), camY - (bottomLeft.y << _n) );
//
//}