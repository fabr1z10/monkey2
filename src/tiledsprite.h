//#pragma once
//
//#include "tilemap.h"
//
//class TiledSprite : public Model<primitives::Quad> {
//
//public:
//    struct FrameInfo {
//        std::shared_ptr<TileMap> model;
//        int ticks;
//    };
//
//    TiledSprite(int batchId, int width, int height);
//
//    void addFrame(const std::string& animation, std::shared_ptr<TileMap>);
//
//    std::shared_ptr<IRenderer> getRenderer(int) override;
//    
//    int getPrimitiveCount() const override;
//
//    TileMap* getTileMap(const std::string& anim, int frame);
//
//    int getTileCount() const;
//
//    std::string getDefaultAnimation() const;
//private:
//    std::unordered_map<std::string, std::vector<FrameInfo>> _frames;
//    int _width;
//    int _height;
//    int _tileCount;
//    int _batchId;
//    std::string _defaultAnimation;
//};
//
//inline std::string TiledSprite::getDefaultAnimation() const {
//    return _defaultAnimation;
//}
//
//inline int TiledSprite::getTileCount() const {
//    return _tileCount;
//}
//
//inline int TiledSprite::getPrimitiveCount() const {
//    return _tileCount;
//}
//
//
//
//class TiledSpriteRenderer : public Renderer<TiledSprite> {
//public:
//    TiledSpriteRenderer(TiledSprite* model, int batchId) : Renderer<TiledSprite>(model, batchId),
//        _frame(0), _tickCount(0), _animation(model->getDefaultAnimation()), 
//        _tileCount(model->getTileCount()) {}
//
//    void setAnimation(const std::string&) override;
//
//    void updateGeometry() override;
//private:
//    std::string _animation;
//    int _frame;
//    int _tickCount;
//    int _tileCount;
//};
