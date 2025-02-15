#pragma once

#include "../shape.h"
#include <vector>

namespace shapes {

    class Line : public Shape {
    public:
        Line(glm::vec2 A, glm::vec2 B);

        bool raycastY(glm::vec2 origin, int dir) const override;

    private:
        float x0, x1;
        float y0, y1;
    };

    class PolyLine : public Shape {
    public:
        PolyLine(const std::vector<float>& data);

        bool raycastY(glm::vec2 origin, int dir) const override;
    private:

        std::vector<float> _x;
        std::vector<float> _y;
    };
}
