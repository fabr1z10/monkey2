#pragma once

#include "collider.h"
#include "../shapes/line.h"
class Baseline : public Collider {

public:

	float getY(float x) const;

	std::shared_ptr<shapes::PolyLine> _polyLine;
};