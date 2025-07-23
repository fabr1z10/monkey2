#pragma once

#include "glm/glm.hpp"
#include "bounds.h"
#include <memory>
#include "geometry.h"

class IModel;

enum ShapeType {
	POINT, LINE, POLYLINE, POLYGON, RECT, AABB
};

enum ModelType {
    WIREFRAME, SOLID
};

class Shape {
public:
    virtual ~Shape() {};

	virtual RaycastResult raycastX(glm::vec2 origin, float length) const = 0;

    virtual RaycastResult raycastY(glm::vec2 origin, float length) const = 0;


	// generate a line model
    virtual std::shared_ptr<IModel> makeModel(int, ModelType type) = 0;

	virtual bool isInside(glm::vec2) const = 0;

	Bounds getBounds() const;

	ShapeType getType() const;
protected:
	ShapeType _type;
	Bounds _bounds;
};

inline Bounds Shape::getBounds() const {
	return _bounds;
}

inline ShapeType Shape::getType() const {
	return _type;
}
