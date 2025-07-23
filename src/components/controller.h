#pragma once

#include "../component.h"
#include "../collisionengine.h"

// old school AGI adventure style
class Controller : public Component {

public:
	Controller(float speed, int mask);

	void start() override;

	void update(double) override;
private:
	float _speed;
	int _mask;
	ICollisionEngine* _engine;
};