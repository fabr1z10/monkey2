#include "rect.h"
#include "../model.h"
#include "../primitives/vertex.h"
#include "../models/linemodel.h"

using namespace shapes;

Rect::Rect(float width, float height, Vec2 anchor) : Shape(), _width(width), _height(height) {
	_type = ShapeType::RECT;
	_bl = glm::vec2(-anchor.x, -anchor.y);
}

bool Rect::isInside(glm::vec2 P) const {
	return (P.x >= _bl.x && P.x <= _bl.x + _width && P.y >= _bl.y && P.y <= _bl.y + _height);
}

RaycastResult Rect::raycastX(glm::vec2 origin, float length) const {
	return {};
}

RaycastResult Rect::raycastY(glm::vec2 origin, float length) const {
	return {};
}

std::shared_ptr<IModel> Rect::makeModel(int batch, ModelType type) {

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
