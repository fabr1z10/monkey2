//#include "sprite.h"
//#include "quadbatch.h"
//#include "error.h"
//
//Quad::Quad(int batchId, IVec4 texCoords, IVec2 anchor) : Model<VertexPalette>(batchId), _anchor(anchor.toGlm()),
//	_texCoords(texCoords.toGlm())
//{
//
//}
//
//Sprite::Sprite(const std::vector<float> &data, int batchId) : Model<primitives::Quad>(), _batchId(batchId) {
//    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
//    float invw = 1.f / textureSize[0];
//    float invh = 1.f / textureSize[1];
//    for (size_t i = 0; i < data.size(); i += 7) {
//        _prims.emplace_back(&data[i], invw, invh);
//    }
//}
//
//Sprite::Sprite(int batchId) : Model<VertexPalette>(batchId)
//{
//    _batchId = batch->getId();
//    auto data = node["data"].as<std::vector<float>>();
//    M_Assert(data.size() % 6 == 0, "[Sprite] data must be a multiple of 6.");
//    int nquads = data.size() / 6;
//    float invw = 1.f / batch->getTextureWidth();
//    float invh = 1.f / batch->getTextureHeight();
//    for (size_t i = 0; i < data.size(); i += 6) {
//        _prims.emplace_back(&data[i], invw, invh, texId);
//    }
//
//    auto anim = node["animations"];
//    auto ticks = node["ticks"].as<int>(20);
//    std::string validAnimation;
//    for (auto an = anim.begin(); an != anim.end(); ++an) {
//        auto animId = an->first.as<std::string>();
//        if (_defaultAnimation.empty()) _defaultAnimation=animId;
//        auto quads = an->second.as<std::vector<int>>(std::vector<int>());
//        if (quads.empty()) {
//            // this is an alias of previous anim
//            _alias[animId] = validAnimation;
//            //_animationFrameCount[animId] = _animationFrameCount.at(validAnimation);
//        } else {
//            validAnimation = animId;
//            AnimInfo info;
//            for (auto i = 0; i <quads.size(); ++i) {
//                if (quads[i] >= 0) {
//                    info.frames.push_back(QuadInfo{quads[i], ticks, false});
//                } else {
//                    switch(quads[i]) {
//                    case -1:
//                        // set loop index
//                        info.loopIndex = quads[++i];
//                        break;
//                    case -2:
//                        info.frames.push_back(QuadInfo{ quads[++i], ticks, true });
//                        break;
//
//                    }
//                }
//            }
//            _animationInfo[animId] = info;
//            //_animationFrameCount[animId] = quads.size();
//        }
//
//
//    }
//
//    if (auto slots = node["slots"]; slots.IsDefined()) {
//        auto slotRaw = slots.as<std::vector<float>>();
//        int floatsPerSlot = nquads * 2;
//        M_Assert(slotRaw.size() % floatsPerSlot == 0, "no");
//        int slotSize = slotRaw.size() / floatsPerSlot;
//        for (size_t j = 0; j < slotSize; ++j) {
//            int offset = j * floatsPerSlot;
//            _slots.push_back({});
//            for (size_t i = 0; i < nquads; i++) {
//                float ax = data[6 * i + 4];
//                float ay = data[6 * i + 5];
//
//                _slots[j].push_back(glm::vec2(slotRaw[offset + 2*i]-ax,
//                    slotRaw[offset + 2*i + 1]-ay));
//            }
//        }
//
//    }
//
//}
//
//Sprite::Sprite(const std::vector<float> &data, int batchId, int textureId) : Model<primitives::Quad>(), _batchId(batchId) {
//    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
//    float invw = 1.f / textureSize[0];
//    float invh = 1.f / textureSize[1];
//    for (size_t i = 0; i < data.size(); i += primitives::Quad::_floatsPerPrimitive - 1) {
//        _prims.emplace_back(&data[i], invw, invh, textureId);
//    }
//}
//
//const Sprite::AnimInfo* Sprite::getAnimationInfo(const std::string& id) {
//    auto it = _animationInfo.find(id);
//    if (it == _animationInfo.end()) {
//        auto aliasId = _alias.find(id);
//        if (aliasId == _alias.end()) {
//            GLIB_FAIL("Don't know animation " + id);
//        }
//        it = _animationInfo.find(aliasId->second);
//
//    }
//    return &it->second;
//}
//
//// void Sprite::add(const std::string &animation, int frame, int quad, int ticks) {
//
//
////     _animationQuads[{animation, frame}] = QuadInfo{quad, ticks};
////     _animationFrameCount[animation] = std::max(_animationFrameCount[animation], frame+1);
//// }
//
//const Sprite::QuadInfo& Sprite::getQuad(const std::string &anim, int frame) const {
//    auto it = _alias.find(anim);
//    std::string a = (it == _alias.end()) ? anim : it->second;
//    M_Assert(_animationInfo.count(a) > 0, ("Unknown animation: " + anim).c_str());
//    return _animationInfo.at(a).frames[frame];
//    // auto it = _animationQuads.find({anim, frame});
//    // if (it == _animationQuads.end()) {
//    //     return {-1, 0};
//    // }
//    // return it->second;
//}
//
//std::shared_ptr<IRenderer> Sprite::getRenderer(int) {
//    return std::make_shared<SpriteRenderer>(this, _batchId);
//
//}
//
//glm::vec2 Sprite::getSlot(const std::string& anim, int frame, int slot) const {
//    auto& q = getQuad(anim, frame);
//    return _slots[slot][q.id];
//}
//
//void SpriteRenderer::updateGeometry() {
//    if (!_started) {
//        return;
//    }
//    const auto& quadInfo = _model->getQuad(_animation, _frame);
//    if (quadInfo.id != -1) {
//        auto worldTransform = _node->getWorldMatrix();
//        _model->get(quadInfo.id).transform(_vertices[0], worldTransform, _multiplyColor, quadInfo.fliph);
//    }
//}
//
//void SpriteRenderer::start() {
//    if (!_started) {
//        Renderer<Sprite>::start();
//        _animInfo = _model->getAnimationInfo(_animation);
//    }
//}
//
//bool SpriteRenderer::updateFrame() {
//    if (!_started || _animInfo == nullptr) {
//        return false;
//    }
//
//    const auto& quadInfo = _animInfo->frames[_frame]; //_model->getQuad(_animation, _frame);
//    if (quadInfo.id != -1) {
//        _tickCounter++;
//        if (_tickCounter >= quadInfo.ticks) {
//            // increment frame
//            _frame++;
//            if (_frame >= _animInfo->frames.size()) {
//                _frame = _animInfo->loopIndex;
//            }
//            _tickCounter = 0;
//            return true;
//        }
//    }
//    return false;
//}
//
//void SpriteRenderer::update() {
//    if (updateFrame()) {
//        updateGeometry();
//    }
//
//}
//
//void SpriteRenderer::setAnimation(const std::string & anim) {
//    if (_animation != anim) {
//        _animation = anim;
//        _frame = 0;
//        _animInfo = _model->getAnimationInfo(anim);
//    }
//
//}
//
//Quad::Quad(const YAML::Node &node, IBatch *batch, int texId)
//{
//    _batchId = batch->getId();
//    auto data = node["data"].as<std::vector<int>>();
//    M_Assert(data.size() == 6, "Quad requires 6 float - x, y, w, h, anchor x, anchor y");
//
//    _prims.emplace_back(
//			batch->pixelToTextureCoords(glm::ivec4(data[0], data[1], data[2], data[3])),
//			glm::vec2(data[4], data[5]),
//			glm::vec2(data[2], data[3]),
//			texId);
//}
//
//PalQuad::PalQuad(IVec4 textureCoords, IVec2 anchor, int palette, int batchId, IVec2 size) : _batchId(batchId), _defaultPalette(palette) {
//	auto batch = Game::instance().getRoom()->getBatch(batchId);
//	glm::vec2 s = size.isZero() ? glm::vec2(textureCoords.z, textureCoords.w) : glm::vec2(size.x, size.y);
//	_prims.emplace_back(
//			batch->pixelToTextureCoords(textureCoords.toGlm()),
//			anchor.toGlm(),
//			s);
//}
//
//
//
//Quad::Quad(const std::vector<float> &data, int batchId, int texId) : Model<primitives::Quad>(), _batchId(batchId) {
//    M_Assert(data.size() == 6, "Quad requires 6 float - x, y, w, h, anchor x, anchor y");
//    auto textureSize  = dynamic_cast<QuadBatch*>(Game::instance().getRoom()->getBatch(batchId))->getTextureSize();
//    float invw = 1.f / textureSize[0];
//    float invh = 1.f / textureSize[1];
//    _prims.emplace_back(&data[0], invw, invh, texId);
//}
//
//std::shared_ptr<IRenderer> Quad::getRenderer(int)
//{
//    return std::make_shared<Renderer<Quad>>(this, _batchId);
//
//}
//
//glm::vec2 SpriteRenderer::getSlot(int id) {
//    return _model->getSlot(_animation, _frame, id);
//}
//
//std::shared_ptr<IRenderer> PalQuad::getRenderer(int)
//{
//	return std::make_shared<PaletteRenderer<PalQuad>>(this, _batchId, _defaultPalette);
//
//}