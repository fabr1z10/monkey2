#include "tilecollider.h"
#include "../model.h"



TileCollider::TileCollider(float width, float height, float gravity, int batch, int verticalRays, int horRays) : Component(),
	_width(width), _height(height), _batchId(batch),_velocity(glm::vec2(0.f)), _gravity(gravity), _verticalRays(verticalRays), _horizontalRays(horRays) {
	
	_stepX = _width / (_verticalRays + 1.f);
	_stepY = _height / (_horizontalRays + 1.f);
	_halfWidth = _width * 0.5f;
	_grounded = false;
}	

void TileCollider::start() {
	
	std::vector<float> debugModelData{
		-_halfWidth, 0.f, 0.f, _halfWidth, 0.f, 0.f,
		_halfWidth, 0.f, 0.f, _halfWidth, _height, 0.f,
		_halfWidth, _height, 0.f, -_halfWidth, _height, 0.f,
		-_halfWidth, _height, 0.f, -_halfWidth, 0.f, 0.f
	};
	auto model = std::make_shared<Model<primitives::Line>>(debugModelData);
	m_node->setModel(model, _batchId);
	_world = dynamic_cast<TileWorld*>(m_node->getParent());
	if (_world == nullptr) {
		std::cerr << "TileController: parent is not a TileWorld\n";
		return;
	}
}


void TileCollider::update(double dt) {
	auto pos = m_node->getWorldPosition();
	auto delta = 0.0167f * _velocity;
	
	// test vertical
	_grounded = false;
	glm::vec2 move(delta);
	if (delta.y != 0.f) {
		float x = pos.x - _halfWidth;
		float y = pos.y + (delta.y < 0 ? 0.f : _height);
		float y1 = y + delta.y;
		for (int i = 0; i < _verticalRays; ++i) {
			auto result = _world->ver(x, pos.y, y1);
			if (result.collide) {
				y1 = result.position;
				move.y = y1 - y;
				_velocity.y = 0.f;
				_grounded = true;
			}
			x += _stepX;
		}
		
	}
	if (delta.x != 0.f) {
		float y = pos.y + move.y;
		float x = pos.x + (delta.x > 0 ? _halfWidth : -_halfWidth);
		float x1 = x + delta.x;
		for (int i = 0; i < _horizontalRays; ++i) {
			auto result = _world->hor(x, y, x1);
			if (result.collide) {
				x1 = result.position;
				move.x = x1 - x;
				_velocity.x = 0.f;
			}
		}
	}
	if (move != glm::vec2(0.f)) {
		m_node->move(glm::vec3(move, 0.f));
	}


	_velocity.y -= _gravity * (float)dt;



}
