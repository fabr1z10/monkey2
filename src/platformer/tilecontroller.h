#pragma once

#include "../component.h"
#include "tileworld.h"

class TileCollider : public Component {

public:
	TileCollider(float width, float height, int batch,
		int verticalRays = 2, int horizontalRays = 2);

	void start() override;

	void update(double delta) override;

	void setVelocityX(float vx);

	void setVelocityY(float vy);
protected:
	float _width;
	float _height;
	int _batchId;
	TileWorld* _world;
	glm::vec2 _velocity;
	int _verticalRays;
	int _horizontalRays;
	float _stepX;
	float _stepY;
	float _halfWidth;
};

inline void TileCollider::setVelocityX(float vx) {
	_velocity.x = vx;
}

inline void TileCollider::setVelocityX(float vy) {
	_velocity.y = vy;
}
