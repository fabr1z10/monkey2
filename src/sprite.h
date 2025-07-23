//#pragma once
//
//#include <vector>
//#include "model.h"
//#include "primitives/vertex.h"
//#include "renderer.h"
//#include "tex.h"
//#include "batch.h"
//#include <yaml-cpp/yaml.h>
//
//class Quad : public Model<VertexPalette> {
//public:
//    Quad(int batchId, IVec4 texCoords, IVec2 anchor = IVec2());
//
//    //Quad(const std::vector<float>& data, int batchId, int texId);
//
//    //std::shared_ptr<IRenderer> getRenderer(int batchId = -1) override;
//private:
//    int _batchId;
//	glm::vec2 _anchor;
//	glm::vec4 _texCoords;
//};
///*
//
//class PalQuad : public Model<primitives::QuadPalette> {
//public:
//	PalQuad(IVec4 textureCoords, IVec2 anchor, int palette, int batchId, IVec2 size = IVec2());
//
//	std::shared_ptr<IRenderer> getRenderer(int batchId = -1) override;
//
//private:
//	int _batchId;
//	int _defaultPalette;
//};
//*/
//
//
//
//
//
//
//class Sprite : public Model<VertexPalette> {
//private:
//	std::string _animation;
//	int _frame;
//public:
//    struct QuadInfo {
//        int id;
//        int ticks;
//        bool fliph;
//    };
//    struct AnimInfo {
//        AnimInfo() : loopIndex(0) {}
//        std::vector<QuadInfo> frames;
//        int loopIndex;
//    };
//
//	Sprite(int batchId);
////
////    Sprite(const std::vector<float>& data, int batchId, int texId);
////
////    Sprite(const std::vector<float>& data, int batchId);
//
//    //void add(const std::string& animation, int frame, int quad, int ticks);
//
//    //std::shared_ptr<IRenderer> getRenderer(int batchId = -1) override;
//
//    int getPrimitiveCount() const override {
//        return 1;
//    }
//
//    std::string getDefaultAnimation() const;
//
//    void setDefaultAnimation(const std::string&);
//
//    const QuadInfo& getQuad(const std::string& anim, int frame) const;
//
//    glm::vec2 getSlot(const std::string& anim, int frame, int slot) const;
//
//    int getAnimationFrames(const std::string& anim) const;
//
//    const AnimInfo* getAnimationInfo(const std::string&);
//private:
//    std::unordered_map<std::string, AnimInfo> _animationInfo;
//    //std::unordered_map<std::pair<std::string, int>, QuadInfo> _animationQuads;
//    //std::unordered_map<std::string, int> _animationFrameCount;
//    std::string _defaultAnimation;
//    std::unordered_map<std::string, std::string> _alias;
//    int _batchId;
//    std::vector<std::vector<glm::vec2>> _slots;
//};
//
//
//
//
//inline std::string Sprite::getDefaultAnimation() const {
//    return _defaultAnimation;
//}
//
//inline int Sprite::getAnimationFrames(const std::string &anim) const {
//    return _animationInfo.at(anim).frames.size();
//}
//
//inline void Sprite::setDefaultAnimation(const std::string & anim) {
//    _defaultAnimation = anim;
//}
//
//class SpriteRenderer : public Renderer<primitives::Quad> {
//public:
//    SpriteRenderer(Sprite* model, int batchId) : Renderer<primitives::Quad>(model, batchId),
//        _animation(model->getDefaultAnimation()), _frame(0), _tickCounter(0) {}
//
//    void start() override;
//
//    void update() override;
//
//    void updateGeometry () override;
//
//    void setAnimation(const std::string&) override;
//
//    std::string getAnimation() const override;
//
//    // returns true if frame changed
//    bool updateFrame();
//
//    glm::vec2 getSlot(int);
//
//    int getFrame() const;
//private:
//    const Sprite::AnimInfo* _animInfo;
//    std::string _animation;
//    int _frame;
//    int _tickCounter;
//};
//
//inline int SpriteRenderer::getFrame() const {
//    return _frame;
//}
//
//inline std::string SpriteRenderer::getAnimation() const {
//    return _animation;
//}
//
