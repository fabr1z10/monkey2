#include "sprite.h"
#include "quadbatch.h"
#include "../error.h"

Sprite::Sprite(const std::vector<float> &data, int batchId) : Model<primitives::Quad>(), _batchId(batchId) {
    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
    float invw = 1.f / textureSize[0];
    float invh = 1.f / textureSize[1];
    for (size_t i = 0; i < data.size(); i += 7) {
        _prims.emplace_back(&data[i], invw, invh);
    }
}

Sprite::Sprite(const YAML::Node &node, QuadBatch *batch, int texId)
{
    _batchId = batch->getId();
    auto data = node["data"].as<std::vector<float>>();
    float invw = 1.f / batch->getTextureWidth();
    float invh = 1.f / batch->getTextureHeight();
    for (size_t i = 0; i < data.size(); i += 6) {
        _prims.emplace_back(&data[i], invw, invh, texId);
    }

    auto anim = node["animations"];
    auto ticks = node["ticks"].as<int>(20);
    std::string validAnimation;
    for (auto an = anim.begin(); an != anim.end(); ++an) {
        auto animId = an->first.as<std::string>();
        if (_defaultAnimation.empty()) _defaultAnimation=animId;
        auto quads = an->second.as<std::vector<int>>(std::vector<int>());
        if (quads.empty()) {
            // this is an alias of previous anim
            _alias[animId] = validAnimation;
            _animationFrameCount[animId] = _animationFrameCount.at(validAnimation);
        } else {
            validAnimation = animId;
            for (auto i = 0; i <quads.size(); ++i) {
                _animationQuads[{animId, i}] = QuadInfo{quads[i], ticks};
            }
            _animationFrameCount[animId] = quads.size();
        }


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
    auto it = _alias.find(anim);
    std::string a = (it == _alias.end()) ? anim : it->second;
    M_Assert(_animationFrameCount.count(a) > 0, ("Unknown animation: " + anim).c_str());
    return _animationQuads.at({a,frame});
    // auto it = _animationQuads.find({anim, frame});
    // if (it == _animationQuads.end()) {
    //     return {-1, 0};
    // }
    // return it->second;
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

Quad::Quad(const YAML::Node &node, QuadBatch *batch, int texId)
{
    _batchId = batch->getId();
    auto data = node["data"].as<std::vector<float>>();
    M_Assert(data.size() == 6, "Quad requires 6 float - x, y, w, h, anchor x, anchor y");

    float invw = 1.f / batch->getTextureWidth();
    float invh = 1.f / batch->getTextureHeight();
    _prims.emplace_back(&data[0], invw, invh, texId);
}

Quad::Quad(const std::vector<float> &data, int batchId, int texId) : Model<primitives::Quad>(), _batchId(batchId) {
    M_Assert(data.size() == 6, "Quad requires 6 float - x, y, w, h, anchor x, anchor y");
    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
    float invw = 1.f / textureSize[0];
    float invh = 1.f / textureSize[1];
    _prims.emplace_back(&data[0], invw, invh, texId);
}

std::shared_ptr<IRenderer> Quad::getRenderer(int)
{
    return std::make_shared<Renderer<Quad>>(this, _batchId);

}
