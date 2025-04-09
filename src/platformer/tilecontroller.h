#pragma once

#include "../component.h"
#include "tileworld.h"

class TileController : public Component {

public:
	TileController(float width, float height, int batch);

	void start() override;

	void update(double delta) override;
protected:
	float _width;
	float _height;
	int _batchId;
	TileWorld* _world;
	glm::vec2 _velocity;
};
