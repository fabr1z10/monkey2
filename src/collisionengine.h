#pragma once

#include "components/collider.h"
#include "glm/glm.hpp"

class CollisionEngine {
public:
    std::vector<Collider*> raycastY(glm::vec2 origin, int direction, int mask);

	//~CollisionEngine();

    void rmCollider(Collider*);

    void addCollider (Collider*);


private:
    std::unordered_set<Collider*> _colliders;
};