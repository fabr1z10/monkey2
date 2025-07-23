#include "line.h"
#include "../error.h"
#include "../model.h"
#include "../geometry.h"
#include "../primitives/vertex.h"
#include "../models/linemodel.h"
#include "../geometry.h"

using namespace shapes;

Line::Line(Vec2 A, Vec2 B) : Shape(), _A(A.toGlm()), _B(B.toGlm()) {
	_type = ShapeType::LINE;
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
	_bounds.xm = std::min(A.x, B.x);
	_bounds.xM = std::max(A.x, B.x);
	_bounds.ym = std::min(A.y, B.y);
	_bounds.yM = std::max(A.y, B.y);

}


bool Line::isInside(glm::vec2 P) const {
	double crossProduct = cross2D(_B - _A, P - _A);
	if (std::fabs(crossProduct) > EPSILON) return false; // Not collinear

	if (P.x + EPSILON < x0 || P.x - EPSILON > x1 ||
		P.y + EPSILON < y0 || P.y - EPSILON > y1)
		return false;

	return true;
}


RaycastResult Line::raycastY(glm::vec2 origin, float length) const {
	return ::rayCastY(_A, _B, origin, length);
}

RaycastResult Line::raycastX(glm::vec2 origin, float length) const {
	return ::rayCastX(_A, _B, origin, length);
}


PolyLine::PolyLine(const std::vector<float> &data) {
	_type = ShapeType::POLYLINE;
    M_Assert(data.size() % 2 == 0, "Polyline needs an even number of floats (2 per point)");
	_bounds.xm = data[0];
	_bounds.xM = data[0];
	_bounds.ym= data[1];
	_bounds.yM = data[1];
    for (size_t i = 0; i< data.size(); i+=2) {
        _x.push_back(data[i]);
        _y.push_back(data[i+1]);
		if (data[i] < _bounds.xm) _bounds.xm = data[i];
		if (data[i+1] < _bounds.ym) _bounds.ym = data[i+1];
		if (data[i] > _bounds.xM) _bounds.xM = data[i];
		if (data[i+1] > _bounds.yM) _bounds.yM = data[i+1];

    }


}

float PolyLine::getY(float x) const {

	if (x <= _x.front()) return _y.front();
	if (x >= _x.back()) return _y.back();
	for (size_t i = 1; i < _x.size(); ++i) {
		if (x <= _x[i]) {
			return _y[i-1] + (_y[i] - _y[i-1]) * (x - _x[i-1]) / (_x[i] - _x[i-1]);
		}
	}
	throw;
}

RaycastResult PolyLine::raycastY(glm::vec2 origin, float length) const
{
	RaycastResult out;
	for (size_t i = 0; i < _x.size()-1; ++i) {
		if (auto result = ::rayCastY(glm::vec2(_x[i], _y[i]), glm::vec2(_x[i+1], _y[i+1]), origin, length)) {
			if (!out.hit || out.distance > result.distance) {
				out = result;
			}
		}
	}
	return out;
}

RaycastResult PolyLine::raycastX(glm::vec2 origin, float length) const
{
	RaycastResult out;
	for (size_t i = 0; i < _x.size()-1; ++i) {
		if (auto result = ::rayCastX(glm::vec2(_x[i], _y[i]), glm::vec2(_x[i+1], _y[i+1]), origin, length)) {
			if (!out.hit || out.distance > result.distance) {
				out = result;
			}
		}
	}
	return out;
}

// mode is ignored for line and polyline
std::shared_ptr<IModel> Line::makeModel(int batch, ModelType) {

	std::vector<float> data {
        x0, y0, x1, y1
	};
	return std::make_shared<models::LineModel>(batch, data, Color(1.f, 1.f, 1.f, 1.f));

	//auto model = std::make_shared<Model<primitives::Line>>(data);

	//return model;

}

std::shared_ptr<IModel> PolyLine::makeModel(int batch, ModelType) {
	std::vector<float> data;
	for (auto i = 0 ; i < _x.size(); ++i) {
        data.insert(data.end(), {_x[i], _y[i]});
	}
	return std::make_shared<models::LineModel>(batch, data, Color(1.f, 1.f, 1.f, 1.f));

	//auto model = std::make_shared<Model<primitives::Line>>(data);
	//return model;
}

bool PolyLine::isInside(glm::vec2 P) const {
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
