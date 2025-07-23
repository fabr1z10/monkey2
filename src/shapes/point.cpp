#include "point.h"
#include "../geometry.h"
#include "../model.h"

using namespace shapes;

Point::Point() : Shape() {
	_type = ShapeType::POINT;
}

RaycastResult Point::raycastX(glm::vec2 origin, float length) const {
	return {};
}

RaycastResult Point::raycastY(glm::vec2 origin, float length) const {
	return {};
}

bool Point::isInside(glm::vec2 P) const {
	return glm::length(P) < EPSILON;
}

std::shared_ptr<IModel> Point::makeModel(int, ModelType) {
//
//	std::vector<float> data {
//        -1, -1, 0.f, 1, -1, 0.f,
//        1, -1, 0.f, 1, 1, 0.f,
//        1, 1, 0.f, -1, 1, 0.f,
//        -1, 1, 0.f, -1, -1, 0.f
//
//	};
//
//	auto model = std::make_shared<RModel<primitives::Line, PaletteRenderer<primitives::Line>>>(data);
//
//	return model;
//
}
