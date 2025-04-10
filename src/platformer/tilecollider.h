#pragma once

#include "../component.h"
#include "tileworld.h"

class TileCollider : public Component {

public:
	TileCollider(float width, float height, float gravity, int batch,
		int verticalRays = 2, int horizontalRays = 2);

	void start() override;

	void update(double delta) override;

	void setVelocityX(float x) {
		_velocity.x = x;
	}
	void setVelocityY(float y) {
		_velocity.y = y;
	}
	bool grounded() const;
protected:
	float _width;
	float _height;
	int _batchId;
	bool _grounded;
	TileWorld* _world;
	glm::vec2 _velocity;
	int _verticalRays;
	int _horizontalRays;
	float _stepX;
	float _stepY;
	float _halfWidth;
	float _gravity;
};


inline bool TileCollider::grounded() const {
	return _grounded;
}