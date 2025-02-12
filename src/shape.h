#pragma once

#include "glm/glm.hpp"

class Shape {
public:
    virtual ~Shape() {};

    virtual bool raycastY(glm::vec2 origin, int dir) const = 0;


};