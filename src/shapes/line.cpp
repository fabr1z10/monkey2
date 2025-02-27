#include "line.h"
#include "../error.h"
#include "../model.h"
#include "../geometry.h"

using namespace shapes;

Line::Line(glm::vec2 A, glm::vec2 B) : Shape(), _A(A), _B(B) {
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


bool Line::isInside(glm::vec2 P) {
	double crossProduct = cross2D(_B - _A, P - _A);
	if (std::fabs(crossProduct) > EPSILON) return false; // Not collinear

	if (P.x + EPSILON < x0 || P.x - EPSILON > x1 ||
		P.y + EPSILON < y0 || P.y - EPSILON > y1)
		return false;

	return true;
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
PolyLine::PolyLine(const std::vector<float> &data) {

    M_Assert(data.size() % 2 == 0, "Polyline needs an even number of floats (2 per point)");

    for (size_t i = 0; i< data.size(); i+=2) {
        _x.push_back(data[i]);
        _y.push_back(data[i+1]);
    }
}

bool PolyLine::raycastY(glm::vec2 origin, int dir) const
{
    if (origin.x < _x.front() || origin.x > _x.back()) {
        return false;
    }

    for (size_t i = 1; i < _x.size(); ++i) {
        if (origin.x <= _x[i]) {
            // point is between x_{i-1} and x_i
            float yp = _y[i-1] + (_y[i] - _y[i-1]) * (origin.x - _x[i-1]) / (_x[i] - _x[i-1]);
            if (dir == 1) {
                return yp > origin.y;
            }
            return yp < origin.y;
        }
    }

    return false;
}


std::shared_ptr<IModel> Line::makeModel(glm::vec4 color) {

	std::vector<float> data {
		x0, y0, 0.f, x1, y1, 0.f, color.r, color.g, color.b, color.a
	};

	auto model = std::make_shared<Model<primitives::Line>>(data);

	return model;

}

std::shared_ptr<IModel> PolyLine::makeModel(glm::vec4 color) {
	std::vector<float> data;
	for (auto i = 0 ; i < _x.size() - 1; ++i) {
		data.insert(data.end(), {_x[i], _y[i], 0.f, _x[i+1], _y[i+1], 0.f, color.r, color.g, color.b, color.a});
	}
	auto model = std::make_shared<Model<primitives::Line>>(data);
	return model;
}

bool PolyLine::isInside(glm::vec2 P) {
	glm::vec2 A(_x[0], _y[0]);
	for (auto i = 0; i < _x.size()-1; ++i) {
		glm::vec2 B(_x[i+1], _y[i+1]);
		auto crossProduct = cross2D(B - A, P - A);
		if (std::fabs(crossProduct) > EPSILON) continue;
		float x0 = std::min(A.x, B.x);
		float x1 = std::max(A.x, B.x);
		float y0 = std::min(A.y, B.y);
		float y1 = std::max(A.y, B.y);
		if (P.x + EPSILON > x0 || P.x - EPSILON < x1 ||
			P.y + EPSILON > y0 || P.y - EPSILON < y1)
			return true;
		A = B;
	}
	return false;
}