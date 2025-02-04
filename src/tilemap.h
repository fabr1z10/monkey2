#pragma once

#include "model.h"
#include "primitives/prim.h"
#include "renderer.h"




class TileMap : public Model<primitives::Quad> {

public:
    struct FrameInfo {
        size_t offset;
        size_t length;
        int ticks;
    };

    TileMap(int batchId, int width, glm::ivec2 tileSize);

    void addFrame(const std::vector<int>& data, int ticks);

    std::shared_ptr<IRenderer> getRenderer(int batchId) override;

    const FrameInfo& getFrameInfo(int) const;

    int getPrimitiveCount() override {
        return _maxFrameLength;
    }
    int getFrameCount() const;

private:
    int _width;
    //int _height;
    glm::ivec2 _tileSize;
    float _tw;
    float _th;
    int _tilesPerRow;
    int _tilesPerCol;
    // for each frame we store offset and length
    std::vector<FrameInfo> _frameInfo;
    size_t _maxFrameLength;
};

inline int TileMap::getFrameCount() const {
    return _frameInfo.size();
}

inline const TileMap::FrameInfo &TileMap::getFrameInfo(int i) const {
    return _frameInfo[i];
}


class TileMapRenderer : public Renderer<TileMap> {
public:
    TileMapRenderer(TileMap* model, int batchId) : Renderer<TileMap>(model, batchId),
        _frame(0), _tickCount(0) {}
    void update() override;
    void updateGeometry() override;
private:
    int _frame;
    int _tickCount;
};




