#include "point.h"
#include "../geometry.h"
#include "../model.h"

using namespace shapes;

Point::Point() : Shape() {
	_type = ShapeType::POINT;
}

bool Point::raycastY(glm::vec2 origin, int dir) const {

	if (fabs(origin.x) < EPSILON) {
		return (dir > 0 && origin.y <=0) || (dir < 0 && origin.y >=0);
	}
	return false;
}

bool Point::isInside(glm::vec2 P) const {
	return glm::length(P) < EPSILON;
}

std::shared_ptr<IModel> Point::makeModel(glm::vec4 color, int) {

	std::vector<float> data {
		-1, -1, 0.f, 1, -1, 0.f, color.r, color.g, color.b, color.a,
		1, -1, 0.f, 1, 1, 0.f, color.r, color.g, color.b, color.a,
		1, 1, 0.f, -1, 1, 0.f, color.r, color.g, color.b, color.a,
		-1, 1, 0.f, -1, -1, 0.f, color.r, color.g, color.b, color.a,

	};

	auto model = std::make_shared<Model<primitives::Line>>(data);

	return model;

}