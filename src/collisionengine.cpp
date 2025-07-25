#include "collisionengine.h"
#include "node.h"
#include <iostream>

void ICollisionEngine::addResponse(const std::string &tag1, const std::string &tag2, std::shared_ptr<CollisionResponse> response) {
	_response[{tag1, tag2}] = response;
	//_response[{tag2, tag1}]= std::make_shared<CollisionResponse>(onStart, onEnd, true);
	addPyRef(response);
}

bool ICollisionEngine::haveResponse(Collider * c1, Collider * c2) {
	return _response.count({ c1->getTag(), c2->getTag() }) || _response.count({ c2->getTag(), c1->getTag() });
}


void ICollisionEngine::onStart(Collider *c1, Collider *c2) {
	auto it = _response.find({ c1->getTag(), c2->getTag() });
	if (it == _response.end()) {
		it = _response.find({ c2->getTag(), c1->getTag() });
		if (it != _response.end()) {
			it->second->onStart(c2->getNode(), c1->getNode());
		}
	}
	else {
		it->second->onStart(c1->getNode(), c2->getNode());
	}
}

void ICollisionEngine::onEnd(Collider* c1, Collider* c2) {
	auto it = _response.find({ c1->getTag(), c2->getTag() });
	if (it == _response.end()) {
		it = _response.find({ c2->getTag(), c1->getTag() });
		if (it != _response.end()) {
			it->second->onEnd(c2->getNode(), c1->getNode());
		}
	}
	else {
		it->second->onEnd(c1->getNode(), c2->getNode());
	}
}

//BasicCollisionEngine::BasicCollisionEngine() : ICollisionEngine() {}
//

//
//void BasicCollisionEngine::addCollider(Collider * c) {
//    _colliders.insert(c);
//}
//
//void BasicCollisionEngine::rmCollider(Collider * c) {
//    _colliders.erase(c);
//}
//
//
//std::vector<Collider*> BasicCollisionEngine::raycastY(glm::vec2 origin, int direction, int mask, Node*, bool) {
//    std::vector<Collider*> out;
//    // origin is provided in world coordinates
//    for (const auto& c : _colliders) {
//
//        // this is the matrix to convert world to local (to the collider) coordinate system
//        auto m = glm::inverse(c->getNode()->getWorldMatrix());
//
//        // origin in local coordinates
//        glm::vec2 Ol = m * glm::vec4(origin, 0.f, 1.f);
//
//        if (c->getShape()->raycastY(Ol, direction)) {
//            out.push_back(c);
//        }
//    }
//    return out;
//}
SpatialHashingCollisionEngine::SpatialHashingCollisionEngine(float width, float height) : ICollisionEngine(),
	_cellWidth(width), _cellHeight(height) {
	_intersector = std::make_unique<Intersection>();
}

RaycastCollision SpatialHashingCollisionEngine::raycastY(glm::vec2 origin, float length, int mask,
    Node* self, bool stopAtFirst) {
	auto hp = hashPosition(origin);

	int ix = floor(origin.x / _cellWidth);
	int iy0 = floor(origin.y / _cellHeight);
	int iy1 = floor((origin.y + length) / _cellHeight);
	//std::cout << origin.y << ", " << length <<", " << iy0 << ", " << iy1 << "\n";
	int cellsToCheck = 1 + std::abs(iy1-iy0);
	std::vector<Collider*> out;
	std::unordered_set<Collider*> checked;
	int inc = length > 0 ? 1 : -1;
	//int iy = iy0;
    for (int i = 0, iy= iy0; i < cellsToCheck; ++i, iy += inc) {
		// checking cell (ix, iy)
		auto it = _colliders.find({ix, iy});
		if (it != _colliders.end()) {
			//auto colliders = _colliders.at({ix, iy});
			for (auto &c: it->second) {
				if ((c->getCollisionFlag() & mask) == 0) continue;
				if (self == c->getNode() || checked.count(c) > 0) continue;
				checked.insert(c);
				// this is the matrix to convert world to local (to the collider) coordinate system
				auto m = glm::inverse(c->getNode()->getWorldMatrix());
				// origin in local coordinates
				glm::vec2 Ol = m * glm::vec4(origin, 0.f, 1.f);

				if (auto result = c->getShape()->raycastY(Ol, length)) {
					return {c, result.hit, result.distance};
				}
			}
		}
    }

	return {};
}

RaycastCollision SpatialHashingCollisionEngine::raycastX(glm::vec2 origin, float length, int mask,
																Node* self, bool stopAtFirst) {
	auto hp = hashPosition(origin);

	int iy = floor(origin.y / _cellHeight);
	int ix0 = floor(origin.x / _cellWidth);
	int ix1 = floor((origin.x + length) / _cellWidth);
	int cellsToCheck = 1 + std::abs(ix1 - ix0);
	int inc = length > 0 ? 1 : -1;
	//int iy = iy0;


	std::vector<Collider*> out;
	std::unordered_set<Collider*> checked;

	for (int i = 0, ix= ix0; i < cellsToCheck; ++i, ix += inc) {
		// checking cell (ix, iy)
		auto it = _colliders.find({ix, iy});
		if (it != _colliders.end()) {
			for (auto &c: it->second) {
				if ((c->getCollisionFlag() & mask) == 0) continue;
				if (self == c->getNode() || checked.count(c) > 0) continue;
				checked.insert(c);
				// this is the matrix to convert world to local (to the collider) coordinate system
				auto m = glm::inverse(c->getNode()->getWorldMatrix());
				// origin in local coordinates
				glm::vec2 Ol = m * glm::vec4(origin, 0.f, 1.f);

				if (auto result = c->getShape()->raycastX(Ol, length)) {
					return {c, result.hit, result.distance};

				}
			}
		}
	}

	return {};
}

void SpatialHashingCollisionEngine::addCollider(Collider * collider) {
	auto b = collider->getBounds();
	auto im = hashPosition({b.xm, b.ym});
	auto iM = hashPosition({b.xM, b.yM});
	_colliderLocations.insert(std::make_pair(collider, ColliderLocation(im.x, iM.x, im.y, iM.y)));
	std::cout << " ** Added collider at (" << im.x << ", " << im.y << ") -> (" << iM.x << ", " << iM.y << ")\n";
	for (int i = im.x; i <= iM.x; ++i) {
		for (int j = im.y; j <= iM.y; ++j) {
			_colliders[{i, j}].insert(collider);
		}
	}
	auto* node = collider->getNode();
	node->registerObserver(this);
	_nodeToCollider[node] = collider;
}

void SpatialHashingCollisionEngine::rmCollider(Collider * c) {
	auto& loc = _colliderLocations.at(c);
	for (int i = loc.xm; i <= loc.xM; ++i) {
		for (int j = loc.ym; j <= loc.yM; ++j) {
			_colliders.at({i, j}).erase(c);
		}
	}
	_colliderLocations.erase(c);
	_nodeToCollider.erase(c->getNode());

}

glm::ivec2 SpatialHashingCollisionEngine::hashPosition(glm::vec2 P) {
	return {floor(P.x / _cellWidth), floor(P.y / _cellHeight)};
}

void SpatialHashingCollisionEngine::onRemove(Node * node) {
	//rmCollider(_nodeToCollider.at(node));
}

void SpatialHashingCollisionEngine::onMove(Node * node) {
	auto* collider = _nodeToCollider.at(node);
	auto b = collider->getBounds();
	auto im = hashPosition({b.xm, b.ym});
	auto iM = hashPosition({b.xM, b.yM});
	auto& currentLocation = _colliderLocations.at(collider);
	if (currentLocation.xm == im.x && currentLocation.xM == iM.x &&
			currentLocation.ym == im.y && currentLocation.yM == iM.y) {
		// nothing to do here
		currentLocation.dirty = true;
		return;
	}
	// if we get here location has changed, so remove current location
	for (int i = currentLocation.xm; i <= currentLocation.xM; ++i) {
		for (int j = currentLocation.ym; j <= currentLocation.yM; ++j) {
			_colliders.at({i, j}).erase(collider);
		}
	}
	_colliderLocations[collider] = ColliderLocation(im.x, iM.x, im.y, iM.y);
	for (int i = im.x; i <= iM.x; ++i) {
		for (int j = im.y; j <= iM.y; ++j) {
			_colliders[{i, j}].insert(collider);
		}
	}
}

void SpatialHashingCollisionEngine::checkCollisions() {
	//std::unordered_map<std::pair<Collider*, Collider*>, CollisionInfo> currentlyCollidingPairs;
	std::unordered_set<glm::ivec3> cellsExamined;

	unsigned collisionChecks = 0;
	int cc = 0;
	std::unordered_set<UPair<Collider*>> _currentCollidingPairs;
	std::unordered_set<Collider*> checked;
		for (auto &[collider, location]: _colliderLocations) {
		// just check collision only if this collider moved with respect to previous position
		if (!location.dirty || !collider->isActive()) continue;
		checked.insert(collider);
		for (int i = location.xm; i <= location.xM; ++i) {
			for (int j = location.ym; j <= location.yM; ++j) {
				// get colliders at (i, j)
				const auto &colliders = _colliders.at(glm::ivec2(i, j));
				if (colliders.size() <= 1) continue;
				for (const auto &current: colliders) {
					// avoid colliding with itself
					if (current == collider || !current->isActive()) continue;
					if (!haveResponse(collider, current)) continue;
					if (((collider->getCollisionMask() & current->getCollisionFlag()) == 0) ||
						((current->getCollisionMask() & collider->getCollisionFlag()) == 0))
						continue;
					// check aabb
					auto c1b = collider->getBounds();
					auto c2b = current->getBounds();
					if (c1b.testAABB(c2b)) {
						bool collide = false;
						if (collider->getShape()->getType() == ShapeType::AABB &&
							current->getShape()->getType() == ShapeType::AABB) {
							// no need to do further checks
							collide = true;
						} else {
							auto *c1s = collider->getShape();
							auto c1t = collider->getNode()->getWorldMatrix();
							auto *c2s = current->getShape();
							auto c2t = current->getNode()->getWorldMatrix();
							collide = _intersector->test(*c1s, c1t, *c2s, c2t);
						}
						if (collide) {
							// proper collision. Handle.
							UPair<Collider*> p(collider, current);
							if (_previouslyCollidingPairs.count(p) == 0) {
								// callback
								onStart(collider, current);
								//_response.at({collider->getTag(), current->getTag()})->onStart(collider->getNode(), current->getNode());
							}
							_currentCollidingPairs.insert(p);


						}

					}

				}
			}

		}
		location.dirty = false;
	}
	// remove previously colliding pairs for which one collider has been checked
	for (auto it = _previouslyCollidingPairs.begin(); it != _previouslyCollidingPairs.end(); ) {
		if (checked.count(it->first) > 0 || checked.count(it->second) > 0) {  // Remove even numbers
			if (_currentCollidingPairs.count(*it) == 0) {
				onEnd(it->first, it->second);
			}
			it = _previouslyCollidingPairs.erase(it); // erase() returns the next valid iterator
		} else {
			++it;
		}
	}

	_previouslyCollidingPairs.insert(_currentCollidingPairs.begin(), _currentCollidingPairs.end());
}


//std::vector<Collider*> BasicCollisionEngine::getColliders(int mask) const {
//
//    std::vector<Collider*> result;
//    for (auto& c : _colliders) {
//        if ((c->getCollisionFlag() & mask) != 0) {
//            result.push_back(c);
//        }
//    }
//    return result;
//}

std::vector<Collider*> SpatialHashingCollisionEngine::getColliders(int mask) const {

    std::vector<Collider*> result;
    for (auto& [key, value] : _colliders) {
        for (auto& c : value) {
            if ((c->getCollisionFlag() & mask) != 0) {
                result.push_back(c);
            }
        }
    }
    return result;
}

