#pragma once

#include "../component.h"
#include "../shape.h"
#include "../bounds.h"
#include "../vec.h"

class Collider : public Component {

public:
	/*
	 * Provide batchId != 1 if you want to plot the collider shape (mostly for debug reasons)
	 */
    Collider(std::shared_ptr<Shape>, int flag, int mask, const std::string& tag, int batchId = -1);

    virtual ~Collider();

    void start() override;

    Shape* getShape();

	// returns collider bounds (AABB) in world coordinates
	Bounds getBounds();

	int getCollisionFlag() const;

	int getCollisionMask() const;

	std::string getTag() const;
private:
	int _flag;
	int _mask;
	std::string _tag;
    std::shared_ptr<Shape> _shape;
	int _batchId;
};

inline Shape *Collider::getShape() {
    return _shape.get();
}

inline int Collider::getCollisionFlag() const {
	return _flag;
}

inline int Collider::getCollisionMask() const {
	return _mask;
}

inline std::string Collider::getTag() const {
	return _tag;
}

class AABBCollider : public Component {
public:
	AABBCollider(int flag, int mask, int tag, float width, float height, Vec2 anchor = Vec2(), int batch=-1);
	void start() override;

private:
	int _batchId;
	int _flag;
	int _mask;
	int _tag;
	Bounds _aabb;
};