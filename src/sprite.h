#pragma once

#include <vector>
#include "model.h"
#include "primitives/prim.h"
#include "renderer.h"
#include "tex.h"
#include "quadbatch.h"
#include <yaml-cpp/yaml.h>

class Quad : public Model<primitives::Quad> {
public:
    Quad(const YAML::Node& node, QuadBatch* batch, int texId);

    Quad(const std::vector<float>& data, int batchId, int texId);

    std::shared_ptr<IRenderer> getRenderer(int batchId = -1) override;
private:
    int _batchId;
};



class Sprite : public Model<primitives::Quad> {

public:
    struct QuadInfo {
        int id;
        int ticks;
    };
    struct AnimInfo {
        AnimInfo() : loopIndex(0) {}
        std::vector<QuadInfo> frames;
        int loopIndex;
    };
    Sprite(const YAML::Node& node, QuadBatch* batch, int texId);

    Sprite(const std::vector<float>& data, int batchId, int texId);

    Sprite(const std::vector<float>& data, int batchId);

    //void add(const std::string& animation, int frame, int quad, int ticks);

    std::shared_ptr<IRenderer> getRenderer(int batchId = -1) override;

    int getPrimitiveCount() override {
        return 1;
    }

    std::string getDefaultAnimation() const;

    void setDefaultAnimation(const std::string&);

    const QuadInfo& getQuad(const std::string& anim, int frame) const;

    int getAnimationFrames(const std::string& anim) const;

    const AnimInfo* getAnimationInfo(const std::string&);
private:
    std::unordered_map<std::string, AnimInfo> _animationInfo;
    //std::unordered_map<std::pair<std::string, int>, QuadInfo> _animationQuads;
    //std::unordered_map<std::string, int> _animationFrameCount;
    std::string _defaultAnimation;
    std::unordered_map<std::string, std::string> _alias;
    int _batchId;
};




inline std::string Sprite::getDefaultAnimation() const {
    return _defaultAnimation;
}

inline int Sprite::getAnimationFrames(const std::string &anim) const {
    return _animationInfo.at(anim).frames.size();
}

inline void Sprite::setDefaultAnimation(const std::string & anim) {
    _defaultAnimation = anim;
}

class SpriteRenderer : public Renderer<Sprite> {
public:
    SpriteRenderer(Sprite* model, int batchId) : Renderer<Sprite>(model, batchId),
        _animation(model->getDefaultAnimation()), _frame(0), _tickCounter(0) {}

    void start() override;

    void update() override;

    void updateGeometry () override;

    void setAnimation(const std::string&) override;

    std::string getAnimation() const override;

private:
    const Sprite::AnimInfo* _animInfo;
    std::string _animation;
    int _frame;
    int _tickCounter;
};


inline std::string SpriteRenderer::getAnimation() const {
    return _animation;
}

