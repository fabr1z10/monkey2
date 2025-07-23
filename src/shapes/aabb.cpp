#include "aabb.h"
#include "../model.h"
#include "../primitives/vertex.h"
#include "../models/linemodel.h"

using namespace shapes;

AABB::AABB(float width, float height) : AABB(width, height, Vec2()) {}

AABB::AABB(float width, float height, Vec2 anchor) : Shape(), _width(width), _height(height) {
	_type = ShapeType::AABB;
	_bl = glm::vec2(-anchor.x, -anchor.y);
	_bounds.xm = -anchor.x;
	_bounds.xM = -anchor.x + width;
	_bounds.ym = -anchor.y;
	_bounds.yM = -anchor.y + height;

}

bool AABB::isInside(glm::vec2 P) const {
	return (P.x >= _bl.x && P.x <= _bl.x + _width && P.y >= _bl.y && P.y <= _bl.y + _height);
}

RaycastResult AABB::raycastX(glm::vec2 origin, float length) const {
	return {};
}

RaycastResult AABB::raycastY(glm::vec2 origin, float length) const {
	return {};
}

std::shared_ptr<IModel> AABB::makeModel(int batch, ModelType type) {

	if (type == ModelType::WIREFRAME) {
		std::vector<float> data {
				_bl.x, _bl.y,
				_bl.x + _width, _bl.y,
				_bl.x + _width, _bl.y + _height,
				_bl.x, _bl.y + _height,
				_bl.x, _bl.y};
		return std::make_shared<models::LineModel>(batch, data, Color(1.f, 1.f, 1.f, 1.f));
		//return std::make_shared<LineModel>(0, data);
	} else {
		std::vector<float> data {
				_bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f,
				_bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f, _bl.x, _bl.y+_height, 0.f
		};
		//return std::make_shared<Model<primitives::Triangle>>(data);
	}

}
