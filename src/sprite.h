#pragma once

#include "model.h"
#include "primitives/prim.h"
#include "renderer.h"


class Sprite : public Model<primitives::Quad> {

public:
    Sprite(const std::vector<float>& data) : Model<primitives::Quad>(data) {}

    void add(const std::string& animation, int frame, int quad);

    std::shared_ptr<IRenderer> getRenderer(int batchId) override;

    int getPrimitiveCount() override {
        return 1;
    }

    std::string getDefaultAnimation() const;

    void setDefaultAnimation(const std::string&);

    int getQuad(const std::string& anim, int frame) const;
private:
    std::unordered_map<std::pair<std::string, int>, int> _animationQuads;
    std::string _defaultAnimation;

};

inline std::string Sprite::getDefaultAnimation() const {
    return _defaultAnimation;
}

inline void Sprite::setDefaultAnimation(const std::string & anim) {
    _defaultAnimation = anim;
}

class SpriteRenderer : public Renderer<Sprite> {
public:
    SpriteRenderer(Sprite* model, int batchId) : Renderer<Sprite>(model, batchId),
        _animation(model->getDefaultAnimation()), _frame(0) {}

    void update() override;
private:
    std::string _animation;
    int _frame;
};




