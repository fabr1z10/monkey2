#include "collisionengine.h"
#include "node.h"
#include <iostream>

void ICollisionEngine::addResponse(const std::string &tag1, const std::string &tag2, pybind11::object onStart,
								   pybind11::object onEnd) {
	_response[{tag1, tag2}] = std::make_shared<CollisionResponse>(onStart, onEnd, false);
	_response[{tag2, tag1}]= std::make_shared<CollisionResponse>(onStart, onEnd, true);
}

bool ICollisionEngine::haveResponse(Collider * c1, Collider * c2) {
	return _response.count({c1->getTag(), c2->getTag()});
}


void CollisionResponse::onStart(Node *n1, Node *n2) {
	if (_onStart) {
		if (_flip) std::swap(n1, n2);
		_onStart(n1, n2);
	}
}

void CollisionResponse::onEnd(Node *n1, Node *n2) {
	if (_onEnd) {
		if (_flip) std::swap(n1, n2);
		_onEnd(n1, n2);
	}
}

BasicCollisionEngine::BasicCollisionEngine() : ICollisionEngine() {}

SpatialHashingCollisionEngine::SpatialHashingCollisionEngine(float width, float height) : ICollisionEngine(),
	_cellWidth(width), _cellHeight(height) {
	_intersector = std::make_unique<Intersection>();
}

void BasicCollisionEngine::addCollider(Collider * c) {
    _colliders.insert(c);
}

void BasicCollisionEngine::rmCollider(Collider * c) {
    _colliders.erase(c);
}


std::vector<Collider*> BasicCollisionEngine::raycastY(glm::vec2 origin, int direction, int mask, Node*) {
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

std::vector<Collider *> SpatialHashingCollisionEngine::raycastY(glm::vec2 origin, int direction, int mask,Node* self) {
	auto hp = hashPosition(origin);
	std::vector<Collider*> out;
	std::unordered_set<Collider*> checked;
	for (const auto& [coords, colliders] : _colliders) {
		if (coords.x == hp.x && (coords.y - hp.y) * direction >= 0 ) {
			for (auto& c : colliders) {
				if (c->getCollisionFlag() & mask == 0) continue;
				if (self == c->getNode() || checked.count(c) > 0) continue;
				checked.insert(c);
				// this is the matrix to convert world to local (to the collider) coordinate system
				auto m = glm::inverse(c->getNode()->getWorldMatrix());

				// origin in local coordinates
				glm::vec2 Ol = m * glm::vec4(origin, 0.f, 1.f);

				if (c->getShape()->raycastY(Ol, direction)) {
					out.push_back(c);
				}
			}
		}


	}
	return out;
}

void SpatialHashingCollisionEngine::addCollider(Collider * collider) {
	auto b = collider->getBounds();
	auto im = hashPosition({b.xm, b.ym});
	auto iM = hashPosition({b.xM, b.yM});
	_colliderLocations.insert(std::make_pair(collider, ColliderLocation(im.x, iM.x, im.y, iM.y)));
	std::cout << " ** Added collider at (" << b.xm << ", " << b.ym << ") -> (" << b.xM << ", " << b.yM<< ")\n";
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
	return glm::ivec2(floor(P.x / _cellWidth), floor(P.y / _cellHeight));
}

void SpatialHashingCollisionEngine::onRemove(Node * node) {
	rmCollider(_nodeToCollider.at(node));
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
						//std::cout << "AABB collide\n";
						// aabb collide -> perform check
						auto *c1s = collider->getShape();
						auto c1t = collider->getNode()->getWorldMatrix();
						auto *c2s = current->getShape();
						auto c2t = current->getNode()->getWorldMatrix();
						if (_intersector->test(*c1s, c1t, *c2s, c2t)) {
							//std::cout << "collision found\n";
							UPair<Collider*> p(collider, current);
							if (_previouslyCollidingPairs.count(p) == 0) {
								// callback
								_response.at({collider->getTag(), current->getTag()})->onStart(collider->getNode(), current->getNode());
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
				_response.at({it->first->getTag(), it->second->getTag()})->
					onEnd(it->first->getNode(), it->second->getNode());
			}
			it = _previouslyCollidingPairs.erase(it); // erase() returns the next valid iterator
		} else {
			++it;
		}
	}

	_previouslyCollidingPairs.insert(_currentCollidingPairs.begin(), _currentCollidingPairs.end());
}
