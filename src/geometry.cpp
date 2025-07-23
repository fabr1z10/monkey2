#include "geometry.h"
#include <iostream>

const float EPSILON = 1e-6f;
const float EPSILON2 = EPSILON * EPSILON;

// Compute signed area to determine polygon orientation
double signedArea(const std::vector<glm::vec2>& polygon) {
    double area = 0;
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (polygon[i].x * polygon[j].y - polygon[j].x * polygon[i].y);
    }
    return area / 2.0;
}

float cross2D(glm::vec2 u, glm::vec2 v) {

    return (u.x * v.y - u.y * v.x);
}

bool pnpoly(const std::vector<glm::vec2>& points, glm::vec2 p, int offset, int nvert) {
    if (nvert == -1) nvert = points.size();

    bool c = false;

    for (int i = offset, j = offset + nvert - 1; i < offset + nvert; j = i++) {
        glm::vec2 vi = points[i];
        glm::vec2 vj = points[j];

        // Check if point is exactly on an edge (avoids missing edge cases)
        if (std::abs(p.y - vi.y) < EPSILON && std::abs(p.x - vi.x) < EPSILON) {
            return true; // Point is exactly on a vertex
        }
        if (std::abs(p.y - vj.y) < EPSILON && std::abs(p.x - vj.x) < EPSILON) {
            return true;
        }

        // Edge crosses horizontal ray at p.y
        if ((vi.y > p.y) != (vj.y > p.y)) {
            float x_intersect = vi.x + (p.y - vi.y) * (vj.x - vi.x) / (vj.y - vi.y);

            // If point is on the edge, return true
            if (std::abs(x_intersect - p.x) < EPSILON) {
                return true;
            }

            // Toggle inside/outside state if ray crosses polygon edge
            if (p.x < x_intersect) {
                c = !c;
            }
        }
    }

    return c;
}


bool seg2seg(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D, glm::vec2 &P) {
    glm::vec2 AB = B - A;
    glm::vec2 DC = C - D;
    glm::vec2 AC = C - A;

    float den = cross2D(AB, DC);
    if (std::abs(den) < EPSILON) return false; // Parallel or collinear

    float t = cross2D(AC, DC) / den;
    if (t <= 0.0f || t >= 1.0f) return false; // No intersection along AB

    float u = cross2D(AB, AC) / den;
    if (u <= 0.0f || u >= 1.0f) return false; // No intersection along CD

    // Compute intersection point
    P = A + t * AB;
    return true;
}




bool isApproxEqual(glm::vec2 A, glm::vec2 B) {
    glm::vec2 diff = B - A;
    return (diff.x * diff.x + diff.y * diff.y) < EPSILON2;
}

bool pointInSegment(glm::vec2 A, glm::vec2 B, glm::vec2 P) {
	float x = cross2D(B - A, P - A);
	if (fabs(x) > EPSILON) return false; // not collinear
	float dot = glm::dot(P-A, B-A);
	if (dot < EPSILON || dot > glm::dot(B-A, B-A)+EPSILON) return false;
	return true;
}


RaycastResult rayCastX(glm::vec2 A, glm::vec2 B, glm::vec2 P, float length) {
	constexpr float EPSILON = 1e-6f;

	// 1. Check if ray and segment Y overlap
	float ymin = std::min(A.y, B.y);
	float ymax = std::max(A.y, B.y);
	if (P.y < ymin - EPSILON || P.y > ymax + EPSILON) {
		return {};
	}

	float dy = B.y - A.y;
	// handle horizontal segment
	if (std::abs(dy) < EPSILON) {
		if (std::abs(P.y - A.y) > EPSILON) {
			return {};
		}
		float xm = std::min(A.x, B.x);
		float xM = std::max(A.x, B.x);
		float rayEnd = P.x + length;
		if (length > 0) {
			if (P.x >= xM) {
				return {};
			} else if (P.x <= xm && rayEnd >= xm) {
				return {true, xm - P.x};
			} else {
				return {true, 0.f};
			}
		} else {
			if (P.x <= xm) {
				return {};
			} else if (P.x >= xM && rayEnd <= xM) {
				return {true, xM - P.x};
			} else {
				return { true, 0.f};
			}
		}
	}
	std::cout << P.x << ", " << P.y << ", " << length << "\n";
	float t = (P.y - A.y) / dy;
	float xi = A.x + t * (B.x - A.x);

	// 4. Check if intersection point lies along the ray
	if (length > 0) {
		if (P.x <= xi && P.x + length >= xi) {
			return {true, xi - P.x};
		} else {
			return {};
		}
	}
	else {
		if (P.x >= xi && P.x + length <= xi) {
			return {true, xi - P.x};
		} else {
			return {};
		}
	}
	return {};
}

RaycastResult rayCastY(glm::vec2 A, glm::vec2 B, glm::vec2 P, float length) {
	constexpr float EPSILON = 1e-6f;

	// 1. Check if ray and segment Y overlap
	float xmin = std::min(A.x, B.x);
	float xmax = std::max(A.x, B.x);
	if (P.x < xmin - EPSILON || P.x > xmax + EPSILON) {
		return {};
	}

	float dx = B.x - A.x;
	// handle horizontal segment
	if (std::abs(dx) < EPSILON) {
		if (std::abs(P.x - A.x) > EPSILON) {
			return {};
		}
		float ym = std::min(A.y, B.y);
		float yM = std::max(A.y, B.y);
		float rayEnd = P.y + length;
		if (length > 0) {
			if (P.y >= yM) {
				return {};
			} else if (P.y <= ym && rayEnd >= ym) {
				return {true, ym - P.y};
			} else {
				return {true, 0.f};
			}
		} else {
			if (P.y <= ym) {
				return {};
			} else if (P.y >= yM && rayEnd <= yM) {
				return {true, yM - P.y};
			} else {
				return { true, 0.f};
			}
		}
	}

	float t = (P.x - A.x) / dx;
	float yi = A.y + t * (B.y - A.y);

	// 4. Check if intersection point lies along the ray
	if (length > 0) {
		if (P.y <= yi && P.y + length >= yi) {
			return {true, yi - P.y};
		} else {
			return {};
		}
	}
	else {
		if (P.y >= yi && P.y + length <= yi) {
			return {true, yi - P.y};
		} else {
			return {};
		}
	}
	return {};
}


float decrementTowardZero(float x, float amount) {
	if (std::abs(x) <= amount)
		return 0.0f;
	return x - std::copysign(amount, x);
}