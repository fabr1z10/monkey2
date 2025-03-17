#include "rect.h"
#include "../model.h"

using namespace shapes;

Rect::Rect(float width, float height, Vec2 anchor) : Shape(), _width(width), _height(height) {
	_type = ShapeType::RECT;
	_bl = glm::vec2(-anchor.x * _width, -anchor.y * _height);
}

bool Rect::isInside(glm::vec2 P) const {
	return (P.x >= _bl.x && P.x <= _bl.x + _width && P.y >= _bl.y && P.y <= _bl.y + _height);
}

bool Rect::raycastY(glm::vec2 origin, int dir) const {
	return true;
}

std::shared_ptr<IModel> Rect::makeModel(ModelType type) {

    if (type == ModelType::WIREFRAME) {
		std::vector<float> data {
            _bl.x, _bl.y, 0.f, _bl.x + _width, _bl.y, 0.f,
            _bl.x + _width, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f,
            _bl.x+_width, _bl.y+_height, 0.f, _bl.x, _bl.y+_height, 0.f,
            _bl.x, _bl.y+ _height, 0.f, _bl.x, _bl.y, 0.f
		};
		return std::make_shared<Model<primitives::Line>>(data);
	} else {
		std::vector<float> data {
            _bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f,
            _bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f, _bl.x, _bl.y+_height, 0.f
		};
		return std::make_shared<Model<primitives::Triangle>>(data);
	}

}
