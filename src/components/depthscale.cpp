#include "depthscale.h"
#include "../game.h"

DepthScale::DepthScale(float y0, float y1, int mask) : Component(),
    _y0(y0), _y1(y1), _mask(mask) {
    k = 1.f / (_y1 - _y0);
}


void DepthScale::start() {
    _engine = Game::instance().getRoom()->getCollisionEngine();
    _lastPos = m_node->getWorldPosition();

    // check if I have a collider
    _collider = m_node->getComponent<Collider>();
    updateZ();

}

void DepthScale::updateZ() {

    float zAdjust = 0;

    glm::vec2 pos = _lastPos;
    float z = (pos.y - _y0) * k;
    if (_collider != nullptr && _collider->getCollisionFlag() == _mask) {
        // if I'm myself a collider, I need to check if I have any collider ABOVE myself
        // To do so, we can check all colliders whose x-range is overlapping
        std::vector<float> xTest;
        auto b = _collider->getBounds();
        xTest.push_back(b.xm);
        xTest.push_back(b.xM);
        std::cout << "I'm a collider: bounds [" << b.xm << ", " << b.xM << "]\n";
        auto colliders = _engine->getColliders(_mask);
        for (auto c : colliders) {
            auto cb = c->getBounds();
            if (cb.xm > b.xm && cb.xm < b.xM) xTest.push_back(cb.xm);
            if (cb.xM > b.xm && cb.xM < b.xM) xTest.push_back(cb.xM);
        }
        size_t nObjects = 0;
        for (auto& t : xTest) {
            auto objects = _engine->raycastY(t + glm::vec2(0,0.1f), 1, _mask, m_node);
            nObjects = std::max(nObjects, objects.size());
        }
        zAdjust = static_cast<float>(nObjects);
    } else {
        auto objects = _engine->raycastY(pos+glm::vec2(0,0.1f), 1, _mask, m_node, true);
        if (!objects.empty()) {
            auto cpos = objects.front()->getNode()->getWorldPosition();
            zAdjust = 1.0f + (cpos.z - (cpos.y - _y0) * k);
        }
        //zAdjust = objects.size();
    }
    std::cerr << "# objects: " << zAdjust << "\n";
    z += zAdjust;
    m_node->setPosition(glm::vec3(pos.x, pos.y, z));
}
void DepthScale::update(double) {
    glm::vec2 pos = m_node->getWorldPosition();
    if (fabs(pos.x-_lastPos.x) > 0.01 || fabs(pos.y - _lastPos.y) > 0.01) {
        _lastPos = pos;
        updateZ();
    }
}
