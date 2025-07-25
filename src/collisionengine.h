#pragma once

#include "components/collider.h"
#include "glm/glm.hpp"
#include "hashpair.h"
#include "shapes/intersection.h"
#include "nodeobserver.h"
#include "upair.h"
#include "pysubclassable.h"

class CollisionResponse : public PySubclassable {
public:
	virtual ~CollisionResponse() = default;

	virtual void onStart(Node*, Node*) = 0;

	virtual void onEnd(Node*, Node*) = 0;

};


// Trampoline class to forward calls to Python
class PyCollisionResponse : public CollisionResponse {
public:
	using CollisionResponse::CollisionResponse;  // Inherit constructors

	void onStart(Node* a, Node* b) override {
		PYBIND11_OVERRIDE_PURE(void, CollisionResponse, onStart, a, b);
	}

	void onEnd(Node* a, Node* b) override {
		PYBIND11_OVERRIDE_PURE(void, CollisionResponse, onEnd, a, b);
	}
};

//class CollisionResponse {
//public:
//	CollisionResponse(pybind11::object onStart,
//					  pybind11::object onEnd, bool flip) : _flip(false) {
//		if (!onStart.is_none()) {
//			_onStart = onStart.cast<pybind11::function>();
//		}
//		if (!onEnd.is_none()) {
//			_onEnd = onEnd.cast<pybind11::function>();
//		}
//	}
//
//
//	void onStart(Node* n1, Node* n2);
//
//	void onEnd(Node* n1, Node* n2);
//private:
//	pybind11::function _onStart;
//	pybind11::function _onEnd;
//	bool _flip;
//};

struct RaycastCollision {
	Collider* collider = nullptr;
	bool hit = false;
	float length = 0.f;



};


// don't need to expose CollisionResponse class
class ICollisionEngine : public NodeObserver {
public:
    virtual ~ICollisionEngine() = default;

	virtual RaycastCollision raycastX(
			glm::vec2 origin,
			float length,
			int mask,
			Node* self=nullptr,
			bool stopAtFirst = false) = 0;

    virtual RaycastCollision raycastY(
			glm::vec2 origin,
			float length,
			int mask,
			Node* self=nullptr,
			bool stopAtFirst = false) = 0;

	virtual void addCollider(Collider*) = 0;

	virtual void rmCollider(Collider*) = 0;

	virtual void checkCollisions() = 0;

    virtual std::vector<Collider*> getColliders(int mask) const = 0;

	bool haveResponse(Collider*, Collider*);

	void addResponse(const std::string& tag1, const std::string& tag2, std::shared_ptr<CollisionResponse>);

	void onStart(Collider*, Collider*);

	void onEnd(Collider*, Collider*);

protected:
	std::unordered_map<std::pair<std::string, std::string>, std::shared_ptr<CollisionResponse>> _response;
};

//class BasicCollisionEngine : public ICollisionEngine {
//public:
//	BasicCollisionEngine();
//
//    std::vector<Collider*> raycastY(
//        glm::vec2 origin, int direction, int mask,
//        Node* self=nullptr, bool stopAtFirst = false) override;
//
//    void rmCollider(Collider*) override;
//
//    void addCollider (Collider*) override;
//
//	virtual void checkCollisions () override {};
//
//    std::vector<Collider*> getColliders(int mask) const override;
//private:
//    std::unordered_set<Collider*> _colliders;
//};

class SpatialHashingCollisionEngine : public ICollisionEngine {
public:
	SpatialHashingCollisionEngine(float width, float height);

	RaycastCollision raycastX(glm::vec2 origin, float length, int mask, Node* self=nullptr, bool stopAtFirst = false) override;


	RaycastCollision raycastY(glm::vec2 origin, float length, int mask, Node* self=nullptr, bool stopAtFirst = false) override;

	void rmCollider(Collider*) override;

	void addCollider (Collider*) override;

	void checkCollisions () override;

	void onMove(Node*) override;

	void onRemove(Node*) override;

	//void update(double) override;
    std::vector<Collider*> getColliders(int mask) const override;
private:
	struct ColliderLocation {
		ColliderLocation() = default;
		ColliderLocation(int xm, int xM, int ym, int yM) : xm(xm), xM(xM), ym(ym), yM(yM), dirty(true) {}
		int xm, xM, ym, yM;
		bool dirty;
	};



	glm::ivec2 hashPosition(glm::vec2);
	// place colliders in the grid
	std::unordered_map<glm::ivec2, std::unordered_set<Collider*>> _colliders;
	std::unordered_map<Collider*, ColliderLocation> _colliderLocations;

	float _cellWidth;
	float _cellHeight;
	std::unique_ptr<Intersection> _intersector;
	std::unordered_map<Node*, Collider*> _nodeToCollider;
	std::unordered_set<UPair<Collider*>> _previouslyCollidingPairs;
};
