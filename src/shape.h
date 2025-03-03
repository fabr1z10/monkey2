#pragma once

#include "glm/glm.hpp"
#include <memory>

class IModel;

class Shape {
public:
    virtual ~Shape() {};

    virtual bool raycastY(glm::vec2 origin, int dir) const = 0;

	// generate a line model
	virtual std::shared_ptr<IModel> makeModel(glm::vec4 color, int mode) = 0;

	virtual bool isInside(glm::vec2) = 0;

};