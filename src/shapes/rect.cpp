#include "rect.h"
#include "../model.h"

using namespace shapes;

Rect::Rect(float width, float height, glm::vec2 anchor) : Shape(), _width(width), _height(height) {
	_bl = glm::vec2(-anchor.x * _width, -anchor.y * _height);
}

bool Rect::isInside(glm::vec2 P) {
	return (P.x >= _bl.x && P.x <= _bl.x + _width && P.y >= _bl.y && P.y <= _bl.y + _height);
}

bool Rect::raycastY(glm::vec2 origin, int dir) const {
	return true;
}

std::shared_ptr<IModel> Rect::makeModel(glm::vec4 color, int mode) {

	if (mode == 0) {
		std::vector<float> data {
			_bl.x, _bl.y, 0.f, _bl.x + _width, _bl.y, 0.f, color.r, color.g, color.b, color.a,
			_bl.x + _width, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f, color.r, color.g, color.b, color.a,
			_bl.x+_width, _bl.y+_height, 0.f, _bl.x, _bl.y+_height, 0.f, color.r, color.g, color.b, color.a,
			_bl.x, _bl.y+ _height, 0.f, _bl.x, _bl.y, 0.f, color.r, color.g, color.b, color.a
		};
		return std::make_shared<Model<primitives::Line>>(data);
	} else {
		std::vector<float> data {
			_bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f, color.r, color.g, color.b, color.a,
			_bl.x, _bl.y, 0.f, _bl.x+_width, _bl.y+_height, 0.f, _bl.x, _bl.y+_height, 0.f, color.r, color.g, color.b, color.a,
		};
		return std::make_shared<Model<primitives::Triangle>>(data);
	}

}