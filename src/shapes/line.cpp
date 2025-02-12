#include "line.h"

using namespace shapes;

Line::Line(glm::vec2 A, glm::vec2 B) : Shape() {
    if (A.x < B.x) {
        x0 = A.x;
        x1 = B.x;
    } else {
        x0 = B.x;
        x1 = A.x;
    }
    if (A.y < B.y) {
        y0 = A.y;
        y1 = B.y;
    } else {
        y0 = B.y;
        y1 = A.y;
    }
}

bool Line::raycastY(glm::vec2 origin, int dir) const {
    if (origin.x < x0 || origin.x > x1) {
        return false;
    }
    // find y corresponding to origin.y
    float yl = y0 + (y1 - y0) * ((origin.x - x0) / (x1 - x0));
    if (dir == 1) {
        return (yl > origin.y);
    } else {
        return (yl < origin.y);
    }
}