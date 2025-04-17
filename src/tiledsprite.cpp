#include "tiledsprite.h"

TiledSprite::TiledSprite(int batchId, int width, int height) : Model<primitives::Quad>(), 
    _width(width), _height(height), _batchId(batchId) {
    _tileCount = _width * _height;


}
void TiledSprite::addFrame(const std::string& animation, std::shared_ptr<TileMap> tileMap) {
    _frames[animation].push_back(FrameInfo{tileMap, 1});
    if (_defaultAnimation.empty()) {
        _defaultAnimation = animation;

    }
}

TileMap* TiledSprite::getTileMap(const std::string& animation, int frame) {
    return _frames[animation][frame].model.get();
}

std::shared_ptr<IRenderer> TiledSprite::getRenderer(int) {
    return std::make_shared<TiledSpriteRenderer>(this, _batchId);

}

void TiledSpriteRenderer::updateGeometry() {
    if (!_started) {
        return;
    }
    const auto& m = _model->getTileMap(_animation, _frame);
    auto worldTransform = _node->getWorldMatrix();
    for (size_t i = 0; i < _tileCount; ++i) {
        m->get(i).transform(_vertices[i], worldTransform, _multiplyColor);
    }
}

void TiledSpriteRenderer::setAnimation(const std::string& anim) {
    if (_animation != anim) {
        _animation = anim;
        _frame = 0;       
    }

}