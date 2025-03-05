#pragma once

#include "../component.h"
#include "../shape.h"
#include "../bounds.h"

class Collider : public Component {

public:
    Collider(std::shared_ptr<Shape>, int flag, int mask, const std::string& tag);

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