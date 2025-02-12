#include "sprite.h"
#include "quadbatch.h"

Sprite::Sprite(const std::vector<float> &data, int batchId) : Model<primitives::Quad>(), _batchId(batchId) {
    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
    float invw = 1.f / textureSize[0];
    float invh = 1.f / textureSize[1];
    for (size_t i = 0; i < data.size(); i += primitives::Quad::_floatsPerPrimitive) {
        _prims.emplace_back(&data[i], invw, invh);
    }
}

Sprite::Sprite(const std::vector<float> &data, int batchId, int textureId) : Model<primitives::Quad>(), _batchId(batchId) {
    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
    float invw = 1.f / textureSize[0];
    float invh = 1.f / textureSize[1];
    for (size_t i = 0; i < data.size(); i += primitives::Quad::_floatsPerPrimitive - 1) {
        _prims.emplace_back(&data[i], invw, invh, textureId);
    }
}

void Sprite::add(const std::string &animation, int frame, int quad, int ticks) {
    _animationQuads[{animation, frame}] = QuadInfo{quad, ticks};
    _animationFrameCount[animation] = std::max(_animationFrameCount[animation], frame+1);
}

const Sprite::QuadInfo& Sprite::getQuad(const std::string &anim, int frame) const {
    auto it = _animationQuads.find({anim, frame});
    if (it == _animationQuads.end()) {
        return {-1, 0};
    }
    return it->second;
}

std::shared_ptr<IRenderer> Sprite::getRenderer(int) {
    return std::make_shared<SpriteRenderer>(this, _batchId);

}

void SpriteRenderer::updateGeometry() {
    if (!_started) {
        return;
    }
    const auto& quadInfo = _model->getQuad(_animation, _frame);
    if (quadInfo.id != -1) {
        auto worldTransform = _node->getWorldMatrix();
        _model->get(quadInfo.id).transform(_vertices[0], worldTransform);
    }
}
void SpriteRenderer::update() {
    if (!_started) {
        return;
    }
    const auto& quadInfo = _model->getQuad(_animation, _frame);
    if (quadInfo.id != -1) {
        _tickCounter++;
        if (_tickCounter >= quadInfo.ticks) {
            // increment frame
            _frame++;
            if (_frame >= _model->getAnimationFrames(_animation)) {
                _frame = 0;
            }
            _tickCounter=0;
            updateGeometry();
        }
    }
}

void SpriteRenderer::setAnimation(const std::string & anim) {
    if (_animation != anim) {
        _animation = anim;
        _frame = 0;
    }

}