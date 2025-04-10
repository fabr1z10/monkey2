#pragma once

#include "../component.h"
#include "tilecollider.h"

class PlayerController : public Component {
public:
	PlayerController(float speed, float jumpSpeed);
	
	void start() override;

	void update(double delta) override;
private:
	float _speed;
	float _jumpSpeed;
	TileCollider* _collider;
};