#include "polygon.h"
#include "../error.h"


using namespace shapes;

Polygon::Polygon(const std::vector<float> &data)
{
    auto n = data.size();
    M_Assert(n % 2 == 0 && n >= 6, "Provide at least 3 points (2 floats per point).");

    _xm = data[0];
    _xM = _xm;
    _ym = data[1];
    _yM = _ym;
    for (size_t i = 0; i <n;i+=2) {
        _points.push_back({data[i], data[i+1]});
        _xm = std::min(_xm, data[i]);
        _xM = std::max(_xM, data[i]);
        _ym = std::min(_ym, data[i+1]);
        _yM = std::max(_yM, data[i+1]);
    }





}

bool Polygon::raycastY(glm::vec2 origin, int dir) const
{
    if (origin.x < _xm || origin.x > _xM) {
        return false;
    }
    // loop through all segments
    auto n = _points.size();
    for (auto i = 0; i < n; ++i) {
        auto j = (i+1) % n;
        if ((origin.x - _points[i].x) * (origin.x - _points[j].x) <= 0) {
            // test - if we get here x is between x1 and x2
            // in this case we want to compute
            float yp = _points[i].y + ((origin.x - _points[i].x) / (_points[j].x - _points[i].x)) * (_points[j].y - _points[i].y);
            if (yp >= origin.y && dir == 1 || yp <= origin.y && dir == -1) {
                return true;
            }
        }
    }
    return false;
}

