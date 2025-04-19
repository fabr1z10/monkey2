#pragma once

#include "node.h"
#include "model.h"
#include "primitives/prim.h"
#include "renderer.h"

enum class ChunkType : uint16_t {
    RLE = 0x8001,               // signals start of an RLE block
    SetWidth = 0x8002,  
    SetYBase = 0x8003,
	SetBackground = 0x8004,
    Loop = 0x8005,
    LoopEnd = 0x8006,
    Skip = 0x8007,
    GoTo = 0x8008,
    Back = 0x8009,
    SetBackgroundLayer = 0x800A
    
    //Region = 0x02,
    //RawLine = 0x03,
    //MoveCursor = 0x04,
    //ChangeZ = 0x05,
    
    //EndOfMap = 0xFF
};


// tileworld
class TileMap : public Node {

public:

    struct Tile {
        uint16_t tileId;
        uint8_t effects;
    };

    /**
     * width = world width in tiles
     * height = world height in tiles
     * tilesize = must be a pow of 2
     */
    TileMap(int width, int height, int tileSize, int batchId);

    // define the tilemap from a string
    void define(const std::string&);

    //void addFrame(const std::vector<int>& data, int ticks);

    //std::shared_ptr<IRenderer> getRenderer(int) override;

    //const FrameInfo& getFrameInfo(int) const;

    void store(int x, int y, uint16_t id, uint8_t fx = 0);

    const Tile* load(int x, int y);

    //int getFrameCount() const;

	glm::ivec2 getScreenTileCount() const {
		return glm::ivec2(_screenTilesPerRow, _screenTilesPerCol);
	};

	void getIndexOfBottomLeftTile(glm::ivec2&, glm::vec2&) const;
	//int getAtlasTilesPerR//ow() const;

	glm::vec4 getTextureCoordinates(int tileId);
private:
	void tle(const std::vector<std::string>&);
    std::unordered_map<std::pair<int, int>, Tile> _tiles;
    // x, y camera position
    float camX;
    float camY;
    int _n;
	int _shiftAtlas;
    int _tileSize;
	glm::vec2 _tileNormalizedLength;
    //void emitTile(int tile, int x, int y);
    void emitTiles(int tile, int count, int x, int y);
    void fillRegion(int tile, int x, int y, int w, int h);
    void addTile(int x, int y, int tile);
    //int _height;
    
    float _tw;
    float _th;
    int _screenTilesPerRow;
    int _screenTilesPerCol;
    int _atlasTilesPerRow;
    int _atlasTilesPerCol;
    
    // for each frame we store offset and length
    //std::vector<FrameInfo> _frameInfo;
    size_t _maxFrameLength;
    int _batchId;
    int _textureId;
    float _z;
    float _yMax;
    int _width;

	uint _cx, _cy;		// current pointer
	uint8_t _fx;
	std::vector<std::string> splitByNewline(const std::string& input);
	void removeSpacesAndTabs(std::string& str);
	std::vector<std::string> splitArgs(const std::string& argStr) ;

	std::unordered_map<std::string, void (TileMap::*)(const std::vector<std::string>&)> _opcodes;

};



inline void TileMap::store(int x, int y, uint16_t id, uint8_t fx) {
    _tiles[{x, y}] = { id, fx };
}

inline const TileMap::Tile* TileMap::load(int x, int y) {
    if (auto it = _tiles.find({ x, y }); it != _tiles.end()) {
        return &it->second;
    }
    return nullptr;
}

class TileMapRenderer : public Renderer<Model<primitives::Quad>> {
public:
    TileMapRenderer(TileMap* node, Model<primitives::Quad>* model, int batch);
//        _frame(0), _tickCount(0) {}
    void update() override;
	void start() override;
    void updateGeometry() override;
private:
	TileMap* _map;
//private:
//    int _frame;
//    int _tickCount;
};
//
//
//
//
