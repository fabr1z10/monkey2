#include "collisionengine.h"
#include "node.h"

void CollisionEngine::addCollider(Collider * c) {
    _colliders.insert(c);
}

void CollisionEngine::rmCollider(Collider * c) {
    _colliders.erase(c);
}


std::vector<Collider*> CollisionEngine::raycastY(glm::vec2 origin, int direction, int mask) {
    std::vector<Collider*> out;
    // origin is provided in world coordinates
    for (const auto& c : _colliders) {

        // this is the matrix to convert world to local (to the collider) coordinate system
        auto m = glm::inverse(c->getNode()->getWorldMatrix());

        // origin in local coordinates
        glm::vec2 Ol = m * glm::vec4(origin, 0.f, 1.f);

        if (c->getShape()->raycastY(Ol, direction)) {
            out.push_back(c);
        }
    }
    return out;
}
