#include "tilemap.h"
#include "game.h"
#include "quadbatch.h"


TileMap::TileMap(int batchId, int width, glm::ivec2 tileSize)  : Model<primitives::Quad>(), _width(width),
    _tileSize(tileSize), _maxFrameLength(0)
{
    auto* batch = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId));
    _tw = tileSize.x / batch->getTextureWidth();
    _th = tileSize.y / batch->getTextureHeight();
    _tilesPerRow = batch->getTextureWidth() / tileSize.x;
    _tilesPerCol = batch->getTextureHeight() / tileSize.y;
}


void TileMap::addFrame(const std::vector<int> &data, int ticks) {
    size_t i{0};
    // for each frame we store offset and length
    int ix{0};
    int iy{0};
    auto offset = _prims.size();
    while (i < data.size()) {
        if (data[i] >= 0) {
            // this is a square tile
            int row = data[i] / _tilesPerCol;
            int col = data[i] % _tilesPerRow;
            glm::vec2 topLeft (col * _tw, row * _th);
            _prims.push_back(primitives::Quad(
                    glm::vec4(topLeft.x, topLeft.y, _tw, _th),
                    glm::vec2(-ix * _tileSize.x, -iy * _tileSize.y),
                    _tileSize,
                    0));
            ix++;
            if (ix >= _width) {
                ix = 0;
                iy++;
            }
            i++;
        }
    }
    auto frameQuads = _prims.size() - offset;
    _frameInfo.push_back(FrameInfo{offset, frameQuads, ticks});
    _maxFrameLength = std::max(_maxFrameLength, frameQuads);
}

std::shared_ptr<IRenderer> TileMap::getRenderer(int batchId) {
    return std::make_shared<TileMapRenderer>(this, batchId);

}


void TileMapRenderer::updateGeometry() {
    const auto& frameInfo = _model->getFrameInfo(_frame);

    auto worldTransform = _node->getWorldMatrix();
    for (auto i = 0; i < frameInfo.length; ++i) {
        _model->get(frameInfo.offset + i).transform(_vertices[i], worldTransform);
    }
}
void TileMapRenderer::update() {
    if (!_started) {
        return;
    }
    const auto& frameInfo = _model->getFrameInfo(_frame);
    _tickCount++;
    if (_tickCount > frameInfo.ticks) {
        // update frame
        auto oldFrame = _frame;
        _frame++;
        _tickCount = 0;
        if (_frame >= _model->getFrameCount()) {
            _frame = 0;
        }
        if (oldFrame != _frame) {
            _dirty = true;
        }
    }
    if (_dirty) {
        updateGeometry();
        _dirty = false;
    }


}