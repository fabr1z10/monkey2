#include "depthscale.h"
#include "../game.h"

DepthScale::DepthScale(float y0, float y1) : Component(), _y0(y0), _y1(y1) {
    k = 1.f / (_y1 - _y0);
}


void DepthScale::start() {
    _engine = Game::instance().getRoom()->getCollisionEngine();
    _lastPos = m_node->getWorldPosition();
    updateZ();
}

void DepthScale::updateZ() {
    glm::vec2 pos = _lastPos;
    float z = (pos.y - _y0) * k;
    auto objects = _engine->raycastY(pos, 1, 0);
    std::cerr << "# objects: " << objects.size() << "\n";
    z += objects.size();
    m_node->setPosition(glm::vec3(pos.x, pos.y, z));
}
void DepthScale::update(double) {
    glm::vec2 pos = m_node->getWorldPosition();
    if (fabs(pos.x-_lastPos.x) > 0.01 || fabs(pos.y - _lastPos.y) > 0.01) {
        _lastPos = pos;
        updateZ();
    }
}
