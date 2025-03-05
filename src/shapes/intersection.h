#pragma once

#include "../shape.h"
#include <unordered_map>
#include "../hashpair.h"

class Intersection {

public:
	Intersection();

	typedef bool(*methodPtr)(const Shape&, const glm::mat4&, const Shape&, const glm::mat4&);

	bool test(const Shape& s1, const glm::mat4& t1, const Shape& s2, const glm::mat4& t2);

protected:
	std::unordered_map<std::pair<ShapeType, ShapeType>, methodPtr> _map;

};

bool PointVsShape(const Shape&, const glm::mat4&, const Shape&, const glm::mat4&);
