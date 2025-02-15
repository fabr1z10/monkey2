#pragma once

#include "../shape.h"
#include <vector>

namespace shapes {

class Polygon : public Shape {
public:
    Polygon(const std::vector<float>& data);

    bool raycastY(glm::vec2 origin, int dir) const override;
private:

    std::vector<glm::vec2> _points;
    float _xm, _xM, _ym, _yM;
};
}
