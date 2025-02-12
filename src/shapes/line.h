#pragma once

#include "../shape.h"

#include "glm/glm.hpp"

namespace shapes {

    class Line : public Shape {
    public:
        Line(glm::vec2 A, glm::vec2 B);

        bool raycastY(glm::vec2 origin, int dir) const override;

    private:
        float x0, x1;
        float y0, y1;
    };
}