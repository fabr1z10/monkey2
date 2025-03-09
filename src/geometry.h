#pragma once

#include "glm/glm.hpp"
#include <vector>

extern const float EPSILON; // Declaration


// Compute signed area to determine polygon orientation
double signedArea(const std::vector<glm::vec2>& polygon);

float cross2D(glm::vec2 u, glm::vec2 v);

/* Checks whether point p lies inside the polygon defined in points
 */
bool pnpoly(const std::vector<glm::vec2>& points, glm::vec2 p, int offset=0, int length=-1);

// Check if segments AB and CD intersect. If they do, return intersection point in P.
bool seg2seg(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D, glm::vec2 &P);

// Tells whether point P lies in segment AB
bool pointInSegment(glm::vec2 A, glm::vec2 B, glm::vec2 P);

bool isApproxEqual(glm::vec2 A, glm::vec2 B);