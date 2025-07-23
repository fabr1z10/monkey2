#include "depthscale.h"
#include "../game.h"
#include "../error.h"

DepthScale::DepthScale(float y0, float y1, int mask) : Component(),
    _y0(y0), _y1(y1), _mask(mask), _valid(false) {
    k = 1.f / (_y1 - _y0);
}


void DepthScale::start() {
    _engine = Game::instance().getRoom()->getCollisionEngine();
    _lastPos = m_node->getWorldPosition().toGlm();

    // check if I have a collider
	_collider = nullptr;
//    _collider = m_node->getComponent<Collider>();
//	if (_collider != nullptr) {
//		_poly = dynamic_cast<shapes::PolyLine *>(_collider->getShape());
//		M_Assert(_poly != nullptr, "Collider associated to depth scale must have a polyline shape! (at least for now...)");
//
//	}

    //updateZ();

}

void DepthScale::updateZ() {

    float zAdjust = 0;

    glm::vec2 pos = _lastPos;
    float z = (pos.y - _y0) * k;
	if (_collider != nullptr && _engine != nullptr) {
		auto colliders = _engine->getColliders(_mask);
		// z adjustment - only if we have colliders with correct mask
		if (!colliders.empty()) {
			int nObjects = 0;

			if (_collider != nullptr && _collider->getCollisionFlag() == _mask) {
				// if I'm myself a collider, I need to check if I have any collider ABOVE myself
				// To do so, we can check all colliders whose x-range is overlapping
				std::vector<float> xTest;
				auto b = _collider->getBounds();
				xTest.push_back(b.xm);
				xTest.push_back(b.xM);
				for (auto c: colliders) {
					auto cb = c->getBounds();
					auto cpos = c->getNode()->getWorldPosition();
					if (cb.xm > b.xM || cb.xM < b.xm) {
						continue;
						// no collision, skip
					}
					if (cb.xm >= b.xm) {
						// test xm
						// need to get y of current polyline at cb.xm
						float yW = _poly->getY(cb.xm - pos.x) + pos.y;
						if (c->getShape()->raycastY(glm::vec2(cb.xm - cpos.x, yW - cpos.y), 1000)) {
							nObjects++;
						}
					}
				}
			} else {
				// just regular character
				for (auto c: colliders) {
					auto cb = c->getBounds();
					if (cb.xm > pos.x || cb.xM < pos.x) continue;
					auto cpos = c->getNode()->getWorldPosition();

					if (c->getShape()->raycastY(glm::vec2(pos.x - cpos.x, pos.y - cpos.y), 1000.f)) {
						nObjects++;
					}
				}
			}
			zAdjust = nObjects;
		}
	}

//        auto objects = _engine->raycastY(pos+glm::vec2(0,0.1f), 1, _mask, m_node, true);
//        if (!objects.empty()) {
//            auto cpos = objects.front()->getNode()->getWorldPosition().toGlm();
//            zAdjust = 1.0f + (cpos.z - (cpos.y - _y0) * k);
//        }
//        //zAdjust = objects.size();
//    }
//    //std::cerr << "# objects: " << zAdjust << "\n";
	z += zAdjust;
	m_node->setPosition(Vec3(pos.x, pos.y, z));
}
void DepthScale::update(double) {
    glm::vec2 pos = m_node->getWorldPosition().toGlm();
    if (!_valid || fabs(pos.x-_lastPos.x) > 0.01 || fabs(pos.y - _lastPos.y) > 0.01) {
        _lastPos = pos;
        _valid = true;
        updateZ();
    }
}
