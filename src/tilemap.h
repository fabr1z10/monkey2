#pragma once

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


class TileMap : public Model<primitives::Quad> {

public:
    struct FrameInfo {
        size_t offset;
        size_t length;
        int ticks;
    };

    TileMap(float dz, const std::vector<uint16_t>& data);

    //void addFrame(const std::vector<int>& data, int ticks);

    std::shared_ptr<IRenderer> getRenderer(int) override;

    const FrameInfo& getFrameInfo(int) const;


    int getFrameCount() const;

private:
    //void emitTile(int tile, int x, int y);
    void emitTiles(int tile, int count, int x, int y);
    void fillRegion(int tile, int x, int y, int w, int h);
    void addTile(int x, int y, int tile);
    //int _height;
    float _tileSize;
    float _tw;
    float _th;
    int _tilesPerRow;
    int _tilesPerCol;
    int _n;
    // for each frame we store offset and length
    std::vector<FrameInfo> _frameInfo;
    size_t _maxFrameLength;
    int _batchId;
    int _textureId;
    float _z;
    float _yMax;
    int _width;
};

inline int TileMap::getFrameCount() const {
    return _frameInfo.size();
}

inline const TileMap::FrameInfo &TileMap::getFrameInfo(int i) const {
    return _frameInfo[i];
}


//class TileMapRenderer : public Renderer<TileMap> {
//public:
//    TileMapRenderer(TileMap* model, int batchId) : Renderer<TileMap>(model, batchId),
//        _frame(0), _tickCount(0) {}
//    void update() override;
//    void updateGeometry() override;
//private:
//    int _frame;
//    int _tickCount;
//};
//
//
//
//
