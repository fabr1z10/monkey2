#include "sprite.h"

void Sprite::add(const std::string &animation, int frame, int quad) {
    _animationQuads[{animation, frame}] = quad;
}

int Sprite::getQuad(const std::string &anim, int frame) const {
    auto it = _animationQuads.find({anim, frame});
    if (it == _animationQuads.end()) {
        return -1;
    }
    return it->second;
}

std::shared_ptr<IRenderer> Sprite::getRenderer(int batchId) {
    return std::make_shared<SpriteRenderer>(this, batchId);

}

void SpriteRenderer::update() {
    if (!_started) {
        return;
    }
    int quadId = _model->getQuad(_animation, _frame);
    if (quadId != -1) {
        auto worldTransform = _node->getWorldMatrix();
        _model->get(quadId).transform(_vertices[0], worldTransform);
    } else {

    }

}